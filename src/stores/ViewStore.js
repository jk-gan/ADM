import { types, getParent } from 'mobx-state-tree';

export const ViewStore = types
  .model({
    page: 'home',
  })
  .views(self => ({
    get ADM() {
      return getParent(self);
    },
  }))
  .actions(self => ({}));
