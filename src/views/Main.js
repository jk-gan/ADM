// ES6 Component
// Import React and ReactDOM
import React, { Component } from 'react';
import styled from 'styled-components';
import { observer, inject } from 'mobx-react'

import Input from '../components/Input';
import Button from '../components/Button';

const Container = styled.div`
  padding: 10px;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
`;
const Title = styled.div`
  font-family: 'Slabo 27px', serif;
  color: #2c3e50;
  font-size: 100px;
`;

@inject('ADM')
@observer
class Main extends Component {
  constructor() {
    super();
  }

  // render method is most important
  // render method returns JSX template
  render() {
    return (
      <Container>
        <Title>ADMz</Title>
        <Input />
        <Button onClick={() => this.props.ADM.downloadStore.addDownload('http://103.1.138.206/files2.codecguide.com/K-Lite_Codec_Pack_1425_Mega.exe')}>Download</Button>
        {/* <PauseAllButton Aria2Module={this.Aria2Module}/>
            <PauseButton Aria2Module={this.Aria2Module} downloadId={1}/>
            <PauseButton Aria2Module={this.Aria2Module} downloadId={2}/> */}
      </Container>
    );
  }
}

export default Main;
