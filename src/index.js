// ES6 Component
// Import React and ReactDOM
import React from 'react';
import ReactDOM from 'react-dom';
import {Provider} from 'mobx-react';

import Main from './views/Main';

import ADMStore from './stores/ADMStore';

const ADM = ADMStore.create(
  {},
  {
    // Injection
  },
);

const App = () => (
  <Provider ADM={ADM}>
    <Main />
  </Provider>
);

// Render to ID content in the DOM
ReactDOM.render(<App />, document.getElementById('content'));
