// ES6 Component
// Import React and ReactDOM
import React, { Component } from 'react';
import styled from 'styled-components';
import { observer, inject } from 'mobx-react'

import Input from '../components/Input';
import Button from '../components/Button';
import { configure, action, observable } from 'mobx';

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

configure({ enforceActions: true })

@inject('ADM')
@observer
class Main extends Component {
  constructor(props) {
    super(props);

    this.addLink = React.createRef();
  }

  @action
  onChange = (event) => {
    this.newLink = event.target.value
  }

  @action
  onKeyUp = (event) => {
    if (event.key === 'Enter') {
      console.log(this.addLink)
      this.addLink.current.click();
    }
  }

  @action
  addDownload = () => {
    this.props.ADM.downloadStore.addDownload(this.newLink)
  }

  @observable newLink = ``;

  // render method is most important
  // render method returns JSX template
  render() {
    return (
      <Container>
        <Title>ADMz</Title>
        <Input newLink={this.newLink} onChange={this.onChange} onKeyUp={this.onKeyUp} />
        <Button innerRef={this.addLink} onClick={this.addDownload}>Download</Button>
        {/* http://103.1.138.206/files2.codecguide.com/K-Lite_Codec_Pack_1425_Mega.exe */}
      </Container>
    );
  }
}

export default Main;
