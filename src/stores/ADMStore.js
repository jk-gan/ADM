import { types, getEnv } from 'mobx-state-tree';
import { DownloadStore } from './DownloadStore';
import { ViewStore } from './ViewStore';

export const ADMStore = types
  .model('ADMStore', {
    downloadStore: types.optional(DownloadStore, {
      sessions: {},
      downloads: {},
    }),
    view: types.optional(ViewStore, {}),
  })
  .views(self => ({
    get sessions() {
      return self.downloadStore.sessions;
    },
  }))
  .actions(self => ({
    afterCreate() {
      self.downloadStore.init();
    },
  }));
