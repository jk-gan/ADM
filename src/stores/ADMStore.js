import {types} from 'mobx-state-tree';
import {DownloadStore} from './DownloadStore';

const ADMStore = types
  .model('ADMStore', {
    downloadStore: types.optional(DownloadStore, {
      sessions: {},
      downloads: {},
    }),
  })
  .actions(self => ({
    afterCreate() {
      self.downloadStore.init();
    },
  }));

export default ADMStore;
