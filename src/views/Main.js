// @flow

import React, {Component} from 'react';
import styled from 'styled-components';
import {observer, inject} from 'mobx-react';
import {configure, action, observable} from 'mobx';
import {remote} from 'electron'; // eslint-disable-line import/no-extraneous-dependencies

import Input from '../components/Input';
import Button from '../components/Button';
import DownloadListView from './DownloadListView';

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

configure({enforceActions: true});

const dialog = remote.getGlobal('dialog');

type Props = {
  ADM: any,
};

@inject('ADM')
@observer
class Main extends Component {
  @observable
  newLink = '';

  constructor(props) {
    super(props);

    const {ADM} = this.props;

    this.addLink = React.createRef();
    this.optionNode = React.createRef();
    this.downloadStore = ADM.downloadStore;
  }

  componentDidMount() {
    window.addEventListener('beforeunload', event => this.onUnload(event));
  }

  componentWillUnmount() {
    window.removeEventListener('beforeunload', event => this.onUnload(event));
  }

  @action
  onChange = event => {
    this.newLink = event.target.value;
  };

  @action
  // eslint-disable-next-line no-unused-vars
  onKeyUp = event => {
    if (event.key === 'Enter') {
      this.addLink.current.click();
    }
  };

  @action
  // eslint-disable-next-line no-unused-vars
  onUnload = event => {
    // the method that will be used for both add and remove event
    this.downloadStore.saveDownloads();
  };

  @action
  addDownload = () => {
    this.downloadStore.addDownload(this.newLink);
  };

  @action
  resumeDownload = () => {
    this.selectedDecorator(this.downloadStore.resumeSelectedDownload);
  };

  @action
  stopSelectedDownload = () => {
    this.downloadStore.stopDownloads();
  };

  @action
  stopAllDownload = () => {
    this.downloadStore.stopDownloads(false);
  };

  @action
  removeDownload = () => {
    this.selectedDecorator(() =>
      dialog.showMessageBox(
        {
          type: 'question',
          title: 'Remove Download',
          buttons: ['ok', 'cancel'],
          message: '',
          checkboxLabel: 'Delete Completed File on Disk',
        },
        (response, checkboxCheck) => {
          if (response === 0) {
            this.downloadStore.removeSelectedDownload(checkboxCheck);
          }
        },
      ),
    );
  };

  @action
  removeCompletedDownload = () => {
    this.downloadStore.removeCompletedDownload();
  };

  props: Props;

  @action
  selectedDecorator(functionCall) {
    if (this.downloadStore.downloadList.find(d => d.selected)) {
      functionCall();
    }
  }

  render() {
    return (
      <Container>
        <Title>ADMz</Title>
        <Input
          type="text"
          placeholder="e.g. www.abc.com"
          newLink={this.newLink}
          onChange={this.onChange}
          onKeyUp={this.onKeyUp}
        />
        <OptionsContainer innerRef={this.optionNode}>
          <Button innerRef={this.addLink} onClick={this.addDownload}>
            Download
          </Button>
          <Button onClick={this.resumeDownload}>Resume</Button>
          <Button onClick={this.stopSelectedDownload}>Stop</Button>
          <Button onClick={this.stopAllDownload}>Stop All</Button>
          <Button onClick={this.removeDownload}>Delete Selected</Button>
          <Button onClick={this.removeCompletedDownload}>
            Clear Completed
          </Button>
        </OptionsContainer>
        <DownloadListView options={this.optionNode} />
      </Container>
    );
  }
}

export default Main;
