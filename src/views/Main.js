// ES6 Component
// Import React and ReactDOM
import React, { Component } from 'react';
import styled from 'styled-components';
import { observer, inject } from 'mobx-react'
import { configure, action, observable } from 'mobx';
import { remote } from 'electron';

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

const OptionsContainer = styled.div`
  display: flex;
  align-items: center;
  justify-content: space-between;
`;

const Title = styled.div`
  font-family: 'Slabo 27px', serif;
  color: #2c3e50;
  font-size: 100px;
`;

configure({ enforceActions: true })

const dialog = remote.getGlobal('dialog');

@inject('ADM')
@observer
class Main extends Component {
  constructor(props) {
    super(props);

    this.addLink = React.createRef();
    this.optionNode = React.createRef();
  }

  componentDidMount() {
    window.addEventListener("beforeunload", (event) => this.onUnload(event))
  }

  componentWillUnmount() {
    window.removeEventListener("beforeunload", (event) => this.onUnload(event))
  }

  @action
  selectedDecorator(functionCall) {
    if (this.props.ADM.downloadStore.downloadList.find(d => d.selected)) {
      functionCall();
    }
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

  @action
  resumeDownload = () => {
    this.selectedDecorator(this.props.ADM.downloadStore.resumeSelectedDownload)
  }

  @action
  removeDownload = () => {
    this.selectedDecorator(() =>
      dialog.showMessageBox({
        type: 'question',
        title: 'Remove Download',
        buttons: ['ok'],
        message: '',
        checkboxLabel: 'Delete Completed File on Disk',
      }, (response, checkboxCheck) => {
        this.props.ADM.downloadStore.removeSelectedDownload(checkboxCheck)
      })
    )
  }

  @action
  removeCompletedDownload = () => {
    this.props.ADM.downloadStore.removeCompletedDownload()
  }

  @observable newLink = `http://103.1.138.206/files2.codecguide.com/K-Lite_Codec_Pack_1425_Mega.exe`;

  render() {
    return (
      <Container>
        <Title>ADMz</Title>
        <Input newLink={this.newLink} onChange={this.onChange} onKeyUp={this.onKeyUp} />
        <OptionsContainer innerRef={this.optionNode}>
          <Button innerRef={this.addLink} onClick={this.addDownload}>Download</Button>
          <Button innerRef={this.addLink} onClick={this.resumeDownload}>Resume</Button>
          <Button innerRef={this.addLink} onClick={this.removeDownload}>Delete Selected</Button>
          <Button innerRef={this.addLink} onClick={this.removeCompletedDownload}>Clear Completed</Button>
        </OptionsContainer>
        <DownloadListView options={this.optionNode} />
        {/* http://103.1.138.206/files2.codecguide.com/K-Lite_Codec_Pack_1425_Mega.exe */}
      </Container>
    );
  }
}

export default Main;
