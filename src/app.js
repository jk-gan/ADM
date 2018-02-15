// ES6 Component
// Import React and ReactDOM
import React, { Component } from 'react'
import ReactDOM from 'react-dom'

import Main from './views/Main'

class App extends Component {
  // constructor() {
  //   super();

  // // Will be move to redux after v0.1
  // this.rem = require('electron').remote;
  // this.Aria2Module = this.rem.getGlobal('Aria2Module');
  // }

  // render method is most important
  // render method returns JSX template
  render() {
    return (
      <Main />
    );
  }
}

// Render to ID content in the DOM
ReactDOM.render(
  <App />,
  document.getElementById('content'),
);
