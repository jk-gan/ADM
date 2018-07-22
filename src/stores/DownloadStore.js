import { values } from 'mobx';
import { types, getParent, flow } from 'mobx-state-tree';
import { remote } from 'electron';
import { generate } from 'shortid';

export const Aria2Module = remote.getGlobal('Aria2Module');

export const downloadState = {
  running: 'running',
  pausing: 'pausing',
  stopped: 'stopped',
};

export const Download = types.model('Download', {
  gid: types.identifier(),
  sessionId: types.string,
  fileName: types.string,
  completedLength: types.number,
  downloadSpeed: types.number,
  uploadSpeed: types.number,
  totalLength: types.number,
});

export const GlobalState = types.model('GlobalState', {
  gNumActive: types.number,
  gNumWaiting: types.number,
  gDownloadSpeed: types.number,
  gUploadSpeed: types.number,
});

export const Session = types.model('Session', {
  id: types.identifier(),
  downloads: types.map(Download),
  gState: GlobalState,
});

export const DownloadStore = types
  .model('DownloadStore', {
    sessions: types.map(Session),
  })
  .views(self => ({
    get ADM() {
      return getParent(self);
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
        download['id'] = sessionId;
        self.sessions.get(sessionId).downloads.put(download);
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
      console.log(dlStatesJson);
      //self.sessions.put(dlStatesJson);
    }

    function completeDownload(completeEventJson) {
      console.log(completeEventJson);
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
