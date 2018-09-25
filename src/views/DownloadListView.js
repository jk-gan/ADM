// @flow

import React, {Component} from 'react';
import {observer, inject} from 'mobx-react';
import {configure, action} from 'mobx';
import {getSnapshot} from 'mobx-state-tree';
import {remote} from 'electron'; // eslint-disable-line import/no-extraneous-dependencies

import ReactTable from 'react-table';
import 'react-table/react-table.css';

configure({enforceActions: true});

const menu = remote.getGlobal('menu');
const MenuItem = remote.getGlobal('MenuItem');
const prompt = remote.getGlobal('prompt');

type Props = {
  ADM: any,
  options: ?HTMLDivElement,
};

@inject('ADM')
@observer
class DownloadListView extends Component {
  static handleRenewUri(store) {
    prompt({
      title: 'Renew Uri',
      label: 'Uri:',
      value: store.getSelectedUri,
      inputAttrs: {
        type: 'url',
      },
      type: 'input',
    })
      .then(result => {
        if (result !== null) {
          store.changeSelectedUri(result);
        }
      })
      .catch(err => {
        throw new Error(err);
      });
  }

  static convertSize(bytes) {
    let size = '';

    if (bytes / 1000 < 1) {
      // bytes
      size = `${bytes}B`;
    } else if (bytes / 1000 < 1000) {
      // kB
      size = `${(bytes / 1000).toFixed(3)}KB`;
    } else if (bytes / 1000 / 1000 < 1000) {
      // mB
      size = `${(bytes / 1000 / 1000).toFixed(3)}MB`;
    } else {
      // gB
      size = `${(bytes / 1000 / 1000 / 1000).toFixed(3)}GB`;
    }

    return size;
  }

  constructor(props) {
    super(props);

    const {ADM} = this.props;

    this.downloadStore = ADM.downloadStore;
    this.createDownloadContextMenu();
  }

  componentDidMount() {
    document.addEventListener(
      'mousedown',
      event => this.handleClick(event),
      false,
    );
  }

  componentWillUnmount() {
    document.removeEventListener(
      'mousedown',
      event => this.handleClick(event),
      false,
    );
  }

  props: Props;

  @action
  handleClick(event) {
    const {options, ADM} = this.props;

    if (
      this.node.contains(event.target) ||
      options.current.contains(event.target)
    ) {
      return;
    }

    ADM.downloadStore.clearAllSelected();
  }

  createDownloadContextMenu() {
    this.resume = new MenuItem({
      label: 'Resume',
      click: () => {
        this.downloadStore.resumeSelectedDownload();
      },
    });
    this.stop = new MenuItem({
      label: 'Stop',
      click: () => {
        this.downloadStore.stopDownloads(true);
      },
    });
    this.renewUri = new MenuItem({
      label: 'Renew URI',
      click: () => DownloadListView.handleRenewUri(this.downloadStore),
    });

    menu.append(this.resume);
    menu.append(this.stop);
    menu.append(this.renewUri);
  }

  render() {
    const columns = [
      {
        id: 'FileName',
        Header: 'File Name',
        width: 350,
        accessor: d => d.fileName.replace(/^.*[\\\/]/, ''), // eslint-disable-line no-useless-escape
      },
      {
        id: 'TotalLength',
        Header: 'File Size',
        width: 80,
        accessor: d => DownloadListView.convertSize(d.totalLength),
      },
      {
        id: 'Percentage',
        Header: 'Percentage',
        width: 80,
        accessor: d =>
          `${((d.completedLength / d.totalLength) * 100).toFixed(2)}%`,
      },
      {
        id: 'DownloadSpeed',
        Header: 'Download',
        width: 80,
        accessor: d => DownloadListView.convertSize(d.downloadSpeed),
      },
      {
        id: 'UploadSpeed',
        Header: 'Upload',
        width: 80,
        accessor: d => DownloadListView.convertSize(d.uploadSpeed),
      },
      {
        accessor: 'state',
        show: false,
      },
      {
        accessor: 'id',
        show: false,
      },
      {
        accessor: 'selected',
        show: false,
      },
      {
        accessor: 'uri',
        show: false,
      },
    ];

    const {ADM} = this.props;

    const data = Object.values(getSnapshot(ADM.downloadStore.downloads));

    return (
      <div
        ref={node => {
          this.node = node;
          // eslint-disable-next-line react/jsx-closing-bracket-location
        }}>
        <ReactTable
          data={data}
          resolveData={resolvedData => resolvedData.map(row => row)}
          columns={columns}
          defaultPageSize={10}
          noDataText="No download found"
          // eslint-disable-next-line no-unused-vars
          getTrProps={(state, rowInfo, column) => {
            if (rowInfo === undefined) {
              return {};
            }

            let backgroundColor = '';
            const {selected} = rowInfo.row;

            if (rowInfo.row.state === 'ERROR') {
              backgroundColor = 'fdd9d7';
            }

            return {
              style: {
                background: selected ? '#cdeefd' : backgroundColor,
              },
              onClick: (e, handleOriginal) => {
                if (!e.ctrlKey && !e.shiftKey) {
                  this.downloadStore.clearAllSelected();
                }

                this.downloadStore.toggleSelectedRow(rowInfo.row.id);

                if (handleOriginal) {
                  handleOriginal();
                }
              },
              // eslint-disable-next-line no-unused-vars
              onContextMenu: (e, handleOriginal) => {
                this.downloadStore.contextMenuSelection(rowInfo.row.id);

                const contextState = this.downloadStore.contextMenuOptions;

                this.resume.enabled = contextState.resume;
                this.stop.enabled = contextState.stop;
                this.renewUri.enabled = contextState.renewUri;

                menu.popup({});
              },
            };
          }}
        />
      </div>
    );
  }
}

export default DownloadListView;
