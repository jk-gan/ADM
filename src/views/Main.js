// ES6 Component
// Import React and ReactDOM
import React, { Component } from 'react'
import styled from 'styled-components'

import Input from '../components/Input'
import Button from '../components/Button'
// import PauseAllButton from './components/pauseAllButton'
// import PauseButton from './components/pauseButton'

const Container = styled.div`
  padding: 10px;
  display: flex; 
  flex-direction: column;
  align-items: center;
  justify-content: center;
`
const Title = styled.div`
  font-family: 'Slabo 27px', serif;
  color: #2c3e50;
  font-size: 100px;
`

class Main extends Component {
  // constructor() {
  //   super();
  // // Will be move to redux after v0.1
  // this.rem = require('electron').remote;
  // this.Aria2Module = this.rem.getGlobal('Aria2Module');
  // }

  onDownload = () => {
    console.log('Download')
  }

  // render method is most important
  // render method returns JSX template
  render() {
    return (
      <Container>
        <Title>ADM</Title>
        <Input />
        <Button onClick={this.onDownload}>Download</Button>
        {/* <PauseAllButton Aria2Module={this.Aria2Module}/>
            <PauseButton Aria2Module={this.Aria2Module} downloadId={1}/>
            <PauseButton Aria2Module={this.Aria2Module} downloadId={2}/> */}
      </Container>
    );
  }
}

export default Main
