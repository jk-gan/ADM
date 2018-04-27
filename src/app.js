// ES6 Component
// Import React and ReactDOM
import React, { Component } from 'react'
import ReactDOM from 'react-dom'
import styled from 'styled-components'

import Input from './components/Input'
// import PauseAllButton from './components/pauseAllButton'
// import PauseButton from './components/pauseButton'

const Container = styled.div`
  padding: 10px;
  display: flex;
  flex-direction: column;
  justify-content: center;
  align-items: center;
`
const Title = styled.div`
  flex: 1;
  font-size: 100px;
  text-align: center;
`

class App extends Component {
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
      <Container>
        <Title>ADM</Title>
        <Input />
        {/* <PauseAllButton Aria2Module={this.Aria2Module}/>
            <PauseButton Aria2Module={this.Aria2Module} downloadId={1}/>
            <PauseButton Aria2Module={this.Aria2Module} downloadId={2}/> */}
      </Container>
    );
  }
}

// Render to ID content in the DOM
ReactDOM.render(
  <App />,
  document.getElementById('content'),
);
