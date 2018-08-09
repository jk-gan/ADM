import { values } from 'mobx';
import { types, getParent, flow } from 'mobx-state-tree';
import { remote } from 'electron';
import { generate } from 'shortid';

const fs = remote.getGlobal('fs');

export const Aria2Module = remote.getGlobal('Aria2Module');

export const Download = types.model('Download', {
  id: types.identifier(),
  gid: types.optional(types.string, ""),
  sessionId: types.string,
  fileName: types.string,
  uri: types.string,
  completedLength: types.number,
  downloadSpeed: types.number,
  uploadSpeed: types.number,
  totalLength: types.number,
  selected: types.optional(types.boolean, false),
  state: types.enumeration('DownloadState', [
    'RUNNING',
    'PAUSING',
    'IDLE',
    'COMPLETED',
    'ERROR'
  ])
});

export const GlobalState = types.model('GlobalState', {
  gNumActive: types.number,
  gNumWaiting: types.number,
  gDownloadSpeed: types.number,
  gUploadSpeed: types.number,
});

export const Session = types.model('Session', {
  id: types.identifier(),
});

export const DownloadStore = types
  .model('DownloadStore', {
    downloads: types.map(Download),
    sessions: types.map(Session)
  })
  .volatile(self => ({
    gStat: GlobalState,
  }))
  .views(self => ({
    get ADM() {
      return getParent(self);
    },
    get downloadList() {
      return values(self.downloads);
    }
  }))
  .actions(self => {

    function addDownload(url) {
      let sessionId = values(self.sessions)[0].id;

      Aria2Module.addDownload(url, sessionId, "", (err, download) => {
        if (err) {
          throw err;

          return;
        }

        let downloadJSON = JSON.parse(download);

        downloadJSON.forEach(download => {
          self.createDownload(sessionId, download, 'RUNNING');
        })
      });
    }

    function resumeSelectedDownload() {
      let sessionId = values(self.sessions)[0].id;

      self.downloads.forEach(download => {
        if (download.selected) {
          Aria2Module.resumeDownload(download.uri, sessionId, download.fileName.replace(/^.*[\\\/]/, ''), (err, downloadData) => {
            if (err) {
              throw err;

              return;
            }

            let downloadJSONArray = JSON.parse(downloadData);

            downloadJSONArray.forEach(downloadJSONData => {
              self.updateDownload(download.id, sessionId, downloadJSONData, 'RUNNING');
            })
          });
        }
      });
    }

    function createSession() {
      Aria2Module.createSession(generate(), (err, sessionId) => {
        if (err) {
          throw err;
        }

        self.updateSession(sessionId);
      });
    }

    function createDownload(sessionId, download, state) {
      download.sessionId = sessionId;
      download.id = generate();
      download.state = state;
      self.downloads.put(download);
    }

    function updateDownload(id, sessionId, download, state) {
      download.sessionId = sessionId;
      download.id = id;
      download.state = state;
      self.downloads.put(download);
    }

    function updateSession(sessionId) {
      self.sessions.put({
        id: sessionId,
        gState: {},
        downloads: [],
      });
    }

    function updateDownloads(dlStatesJson) {
      self.gStat = dlStatesJson.gStats;

      if (dlStatesJson.Downloads === undefined) {
        return;
      }

      dlStatesJson.Downloads.forEach(download => {
        let downloadFind = [...self.downloads].find(([, x]) => x.gid == download.gid);

        if (downloadFind === undefined) {
          return;
        }

        let currDownload = self.downloads.get(downloadFind[0]);

        download.id = downloadFind[0];
        download.state = 'RUNNING';
        download.selected = currDownload.selected

        self.downloads.put(download);
      })
    }

    function stopSelectedDownload() {
      let sessionId = values(self.sessions)[0].id;

      self.downloads.forEach(download => {
        if (download.selected) {
          if (download.gid !== "") {
            Aria2Module.stopDownload(download.sessionId, download.gid, false);
          }
        }
      });
    }

    function removeSelectedDownload() {
      let sessionId = values(self.sessions)[0].id;

      self.downloads.forEach(download => {
        if (download.selected) {
          if (download.gid !== "") {
            // Download session is running. Stop download first
            Aria2Module.stopDownload(download.sessionId, download.gid, false);
          }

          // Remove physical file if ticked
          fs.unlink(download.fileName, (err) => {
            if (err) {
              throw (err);
            }

            fs.state(`${download.fileName}.aria2`, (err, state) => {
              if (err) {
                throw err;
              }

              fs.unlink(`${download.fileName}.aria2`, (err) => {
                if (err) {
                  throw err;
                }
              })
            })
          })

          self.downloads.delete(download.id);
        }
      });
    }

    function completeDownload(completeEventJson) {
      if (completeEventJson.event == "COMPLETE") {
        let downloadFind = [...self.downloads].find(([, x]) => x.gid == completeEventJson.gid);

        if (downloadFind === undefined) {
          return;
        }

        let currDownload = self.downloads.get(downloadFind[0]);

        currDownload.state = 'COMPLETED';
        currDownload.fileName = completeEventJson.filename;
        currDownload.completedLength = currDownload.totalLength;
        currDownload.downloadSpeed = 0;
        currDownload.uploadSpeed = 0;
        currDownload.gid = "";

      } else if (completeEventJson.event == "ERROR") {
        let downloadFind = [...self.downloads].find(([, x]) => x.gid == completeEventJson.gid);

        if (downloadFind === undefined) {
          return;
        }

        let currDownload = self.downloads.get(downloadFind[0]);

        currDownload.state = 'ERROR';
        currDownload.completedLength = currDownload.totalLength;
        currDownload.downloadSpeed = 0;
        currDownload.uploadSpeed = 0;
        currDownload.gid = "";
      }
    }

    function loadDownloads() {
      let downloadsArray = JSON.parse(localStorage.getItem("downloads"));

      if (downloadsArray == undefined) {
        return;
      }

      downloadsArray.forEach(download => {
        self.createDownload('', download, download.completedLength == download.totalLength ? 'COMPLETED' : 'IDLE');
      });
    }

    function saveDownloads() {
      if (typeof (Storage) !== "undefined") {
        let downloadsArray = values(self.downloads);

        downloadsArray.forEach(download => {
          download.selected = false;
          download.downloadSpeed = 0;
          download.uploadSpeed = 0;
          download.gid = "";
        })

        let stringifiedDownloads = JSON.stringify(downloadsArray);

        localStorage.setItem("downloads", stringifiedDownloads);
      } else {
        throw ("ERROR: Web Storage is not avaialble.");
      }
    }

    function removeCompletedDownload() {
      self.downloads.forEach(download => {
        if (download.completedLength == download.totalLength) {
          self.downloads.delete(download.id);
        }
      });
    }

    function toggleSelectedRow(id) {
      let download = self.downloads.get(id);
      download.selected = !download.selected;

      self.downloads.put(download);
    }

    function clearAllSelected() {
      self.downloads.forEach(download => {
        download.selected = false;
      })
    }

    function startMonitoring() {
      Aria2Module.startMonitoring(downloadStatsCallback, completeEventCallback);
    }

    function stopMonitoring() {
      Aria2Module.stopMonitoring();
    }

    function downloadStatsCallback(downloadStats) {
      self.updateDownloads(JSON.parse(downloadStats));
    }

    function completeEventCallback(completeEvent) {
      self.completeDownload(JSON.parse(completeEvent));
    }

    return {
      updateDownloads,
      updateDownload,
      updateSession,
      startMonitoring,
      addDownload,
      resumeSelectedDownload,
      createSession,
      createDownload,
      completeDownload,
      loadDownloads,
      saveDownloads,
      removeSelectedDownload,
      removeCompletedDownload,
      toggleSelectedRow,
      clearAllSelected
    };
  });
