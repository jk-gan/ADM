// ES6 Component
// Import React and ReactDOM
import React, { Component } from 'react';
import styled from 'styled-components';
import { observer, inject } from 'mobx-react'
import { configure, action, observable } from 'mobx';

import Input from '../components/Input';
import Button from '../components/Button';
import DownloadListView from '../views/DownloadListView';

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

  componentDidMount() {
    window.addEventListener("beforeunload", (event) => this.onUnload(event))
  }

  componentWillUnmount() {
    window.removeEventListener("beforeunload", (event) => this.onUnload(event))
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
  onUnload(event) { // the method that will be used for both add and remove event
    this.props.ADM.downloadStore.saveDownloads();
  }

  @action
  addDownload = () => {
    this.props.ADM.downloadStore.addDownload(this.newLink)
  }

  @observable newLink = `http://103.1.138.206/files2.codecguide.com/K-Lite_Codec_Pack_1425_Mega.exe`;

  // render method is most important
  // render method returns JSX template
  render() {
    return (
      <Container>
        <Title>ADMz</Title>
        <Input newLink={this.newLink} onChange={this.onChange} onKeyUp={this.onKeyUp} />
        <Button innerRef={this.addLink} onClick={this.addDownload}>Download</Button>
        <DownloadListView />
        {/* http://103.1.138.206/files2.codecguide.com/K-Lite_Codec_Pack_1425_Mega.exe */}
      </Container>
    );
  }
}

export default Main;
