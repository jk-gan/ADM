// ES6 Component
// Import React and ReactDOM
import React from 'react';
import ReactDOM from 'react-dom';

import Input from './components/input.component'
import PauseAllButton from './components/pauseAllButton'
import PauseButton from './components/pauseButton'


class App extends React.Component {
    constructor() {
      super();

      // // Will be move to redux after v0.1
      // this.rem = require('electron').remote;
      // this.Aria2Module = this.rem.getGlobal('Aria2Module');
    }
    // render method is most important
    // render method returns JSX template
    render() {
        return (
          <div>
            <Input />
            {/* <PauseAllButton Aria2Module={this.Aria2Module}/>
            <PauseButton Aria2Module={this.Aria2Module} downloadId={1}/>
            <PauseButton Aria2Module={this.Aria2Module} downloadId={2}/> */}
          </div>
        );
    }
}

// Render to ID content in the DOM
ReactDOM.render( <App /> ,
    document.getElementById('content')
);
