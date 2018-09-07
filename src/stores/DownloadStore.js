import {values} from 'mobx';
import {types, getParent} from 'mobx-state-tree';
import {remote} from 'electron'; // eslint-disable-line import/no-extraneous-dependencies
import {generate} from 'shortid';

const fs = remote.getGlobal('fs');

export const Aria2Module = remote.getGlobal('Aria2Module');

export const Download = types.model('Download', {
  id: types.identifier(),
  gid: types.optional(types.string, ''),
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
    'ERROR',
  ]),
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
    sessions: types.map(Session),
  })
  // eslint-disable-next-line no-unused-vars
  .volatile(self => ({
    gStat: GlobalState,
  }))
  .views(self => ({
    get ADM() {
      return getParent(self);
    },
    get downloadList() {
      return values(self.downloads);
    },
    get contextMenuOptions() {
      // Return context menu options
      // Set resume and stop disable first
      let resumeState = false;
      let stopState = false;

      let renewUriStateCounter = 0;

      self.downloads.forEach(download => {
        if (download.selected) {
          switch (download.state) {
            case 'RUNNING':
              stopState = true;
              break;

            case 'PAUSING':
            case 'IDLE':
            case 'ERROR':
              resumeState = true;
              break;

            default:
              resumeState = false;
              stopState = false;
          }

          renewUriStateCounter += 1;
        }
      });

      return {
        resume: resumeState,
        stop: stopState,
        renewUri: renewUriStateCounter === 1,
      };
    },
    get getSelectedUri() {
      let uri = '';

      self.downloads.forEach(download => {
        if (download.selected) {
          ({uri} = download);
        }
      });

      return uri;
    },
  }))
  .actions(self => {
    function addDownload(url) {
      const sessionId = values(self.sessions)[0].id;

      Aria2Module.addDownload(url, sessionId, '', (err, serializedDownload) => {
        if (err) {
          throw err;
        }

        const downloadJSON = JSON.parse(serializedDownload);

        downloadJSON.forEach(download => {
          self.createDownload(sessionId, download, 'RUNNING');
        });
      });
    }

    function resumeSelectedDownload() {
      const sessionId = values(self.sessions)[0].id;

      self.downloads.forEach(download => {
        if (
          download.selected &&
          download.state !== 'RUNNING' &&
          download.state !== 'COMPLETED'
        ) {
          Aria2Module.resumeDownload(
            download.uri,
            sessionId,
            download.fileName.replace(/^.*[\\\/]/, ''), // eslint-disable-line no-useless-escape
            (err, downloadData) => {
              if (err) {
                throw err;
              }

              const downloadJSONArray = JSON.parse(downloadData);

              downloadJSONArray.forEach(downloadJSONData => {
                self.updateDownload(
                  download.id,
                  sessionId,
                  downloadJSONData,
                  'RUNNING',
                );
              });
            },
          );
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
      const downloadData = download;

      downloadData.sessionId = sessionId;
      downloadData.id = generate();
      downloadData.state = state;
      self.downloads.put(downloadData);
    }

    function updateDownload(id, sessionId, download, state) {
      const downloadData = download;

      downloadData.sessionId = sessionId;
      downloadData.id = id;
      downloadData.state = state;
      self.downloads.put(downloadData);
    }

    function updateSession(sessionId) {
      self.sessions.put({
        id: sessionId,
        gState: {},
        downloads: [],
      });
    }

    function updateDownloads(dlStatesJson) {
      // eslint-disable-next-line no-param-reassign
      self.gStat = dlStatesJson.gStats;

      if (dlStatesJson.Downloads === undefined) {
        return;
      }

      dlStatesJson.Downloads.forEach(download => {
        const downloadData = download;
        const downloadFind = [...self.downloads].find(
          ([, x]) => x.gid === downloadData.gid,
        );

        if (downloadFind === undefined) {
          return;
        }

        const currDownload = self.downloads.get(downloadFind[0]);

        [downloadData.id] = downloadFind;
        downloadData.gid = currDownload.gid;
        downloadData.state = currDownload.state;
        downloadData.selected = currDownload.selected;

        self.downloads.put(downloadData);
      });
    }

    function stopDownloads(selected = true) {
      self.downloads.forEach(download => {
        const downloadData = download;

        if (downloadData.selected || !selected) {
          if (downloadData.gid !== '') {
            Aria2Module.stopDownload(
              downloadData.sessionId,
              downloadData.gid,
              false,
            );

            // Change download state back to IDLE
            downloadData.state = 'IDLE';
            self.downloads.put(downloadData);
          }
        }
      });
    }

    function completeDownload(completeEventJson) {
      if (completeEventJson.event === 'COMPLETE') {
        const downloadFind = [...self.downloads].find(
          ([, x]) => x.gid === completeEventJson.gid,
        );

        if (downloadFind === undefined) {
          return;
        }

        const currDownload = self.downloads.get(downloadFind[0]);

        currDownload.state = 'COMPLETED';
        currDownload.fileName = completeEventJson.filename;
        currDownload.completedLength = currDownload.totalLength;
        currDownload.downloadSpeed = 0;
        currDownload.uploadSpeed = 0;
        currDownload.gid = '';
      } else if (completeEventJson.event === 'ERROR') {
        const downloadFind = [...self.downloads].find(
          ([, x]) => x.gid === completeEventJson.gid,
        );

        if (downloadFind === undefined) {
          return;
        }

        const currDownload = self.downloads.get(downloadFind[0]);

        currDownload.state = 'ERROR';
        currDownload.completedLength = currDownload.totalLength;
        currDownload.downloadSpeed = 0;
        currDownload.uploadSpeed = 0;
        currDownload.gid = '';
      }
    }

    function loadDownloads() {
      const downloadsArray = JSON.parse(localStorage.getItem('downloads'));

      if (downloadsArray === undefined) {
        return;
      }

      downloadsArray.forEach(download => {
        self.createDownload(
          '',
          download,
          download.completedLength === download.totalLength
            ? 'COMPLETED'
            : 'IDLE',
        );
      });
    }

    function saveDownloads() {
      if (typeof Storage !== 'undefined') {
        const downloadsArray = values(self.downloads);

        downloadsArray.forEach(download => {
          const downloadData = download;

          downloadData.selected = false;
          downloadData.downloadSpeed = 0;
          downloadData.uploadSpeed = 0;
          downloadData.gid = '';
        });

        const stringifiedDownloads = JSON.stringify(downloadsArray);

        localStorage.setItem('downloads', stringifiedDownloads);
      } else {
        throw new Error('ERROR: Web Storage is not avaialble.');
      }
    }

    function removeCompletedDownload() {
      self.downloads.forEach(download => {
        if (download.completedLength === download.totalLength) {
          self.downloads.delete(download.id);
        }
      });
    }

    function removeSelectedDownload(checked) {
      self.downloads.forEach(download => {
        if (download.selected) {
          if (download.gid !== '') {
            // Download session is running. Stop download first
            Aria2Module.stopDownload(download.sessionId, download.gid, false);
          }

          const path = download.fileName;

          if (checked || download.state !== 'COMPLETED') {
            // Remove physical file if ticked
            fs.unlink(path, unlinkErr => {
              if (unlinkErr) {
                throw unlinkErr;
              }

              // eslint-disable-next-line no-unused-vars
              fs.stat(`${path}.aria2`, (statErr, state) => {
                if (statErr) {
                  // File not exist
                  return;
                }

                fs.unlink(`${path}.aria2`, err => {
                  if (err) {
                    throw err;
                  }
                });
              });
            });
          }

          self.downloads.delete(download.id);
        }
      });
    }

    function changeSelectedUri(uri) {
      self.downloads.forEach(download => {
        const downloadData = download;
        if (downloadData.selected) {
          downloadData.uri = uri;

          self.downloads.put(downloadData);
        }
      });
    }

    function toggleSelectedRow(id) {
      const download = self.downloads.get(id);
      download.selected = !download.selected;

      self.downloads.put(download);
    }

    function contextMenuSelection(id) {
      const download = self.downloads.get(id);

      if (!download.selected) {
        self.clearAllSelected();

        download.selected = !download.selected;
      }

      self.downloads.put(download);
    }

    function clearAllSelected() {
      self.downloads.forEach(download => {
        // eslint-disable-next-line no-param-reassign
        download.selected = false;
      });
    }

    function downloadStatsCallback(downloadStats) {
      self.updateDownloads(JSON.parse(downloadStats));
    }

    function completeEventCallback(completeEvent) {
      self.completeDownload(JSON.parse(completeEvent));
    }

    function startMonitoring() {
      Aria2Module.startMonitoring(downloadStatsCallback, completeEventCallback);
    }

    function init() {
      self.loadDownloads();
      self.startMonitoring();
      self.createSession();
    }

    return {
      init,
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
      changeSelectedUri,
      stopDownloads,
      toggleSelectedRow,
      clearAllSelected,
      contextMenuSelection,
    };
  });
