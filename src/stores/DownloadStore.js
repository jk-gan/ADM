import { values } from 'mobx';
import { types, getParent, flow } from 'mobx-state-tree';
import { remote } from 'electron';
import { generate } from 'shortid';

export const Aria2Module = remote.getGlobal('Aria2Module');

export const downloadState = {
  running: 'running',
  pausing: 'pausing',
  stopped: 'stopped',
  completed: 'completed',
  error: 'error',
};

export const Download = types.model('Download', {
  id: types.identifier(),
  gid: types.string,
  sessionId: types.string,
  fileName: types.string,
  completedLength: types.number,
  downloadSpeed: types.number,
  uploadSpeed: types.number,
  totalLength: types.number,
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
    sessions: types.map(Session),
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
    },
  }))
  .actions(self => {
    function addDownload(url) {
      let sessionId = values(self.sessions)[0].id;

      Aria2Module.addDownload(url, sessionId, (err, download) => {
        if (err) {
          console.err(err);
        }

        let downloadJSON = JSON.parse(download);

        self.createDownload(sessionId, downloadJSON);
      });
    }

    function createSession() {
      Aria2Module.createSession(generate(), (err, sessionId) => {
        if (err) {
          console.err(err);
        }

        self.updateSession(sessionId);
      });
    }

    function createDownload(sessionId, downloadJSON) {
      downloadJSON.forEach(download => {
        download['sessionId'] = sessionId;
        download['id'] = generate();
        download['state'] = 'RUNNING';
        self.downloads.put(download);
      })
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

        download['id'] = downloadFind[0];
        download['state'] = 'RUNNING';

        self.downloads.put(download);
      })
    }

    function completeDownload(completeEventJson) {
      if (completeEventJson.event == "COMPLETE") {
        let downloadFind = [...self.downloads].find(([, x]) => x.gid == completeEventJson.gid);

        if (downloadFind === undefined) {
          return;
        }

        let currDownload = self.downloads.get(downloadFind[0]);

        currDownload.state = 'COMPLETED';
        currDownload.completedLength = currDownload.totalLength;
        currDownload.downloadSpeed = 0;
        currDownload.uploadSpeed = 0;
      }
    }

    function loadDownloads() {
      // Load from database
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
      updateSession,
      loadDownloads,
      startMonitoring,
      addDownload,
      createSession,
      createDownload,
      completeDownload
    };
  });
