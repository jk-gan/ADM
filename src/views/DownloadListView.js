import React, { Component } from 'react';
import { observer, inject } from 'mobx-react'
import { configure, action, observable } from 'mobx';
import { getSnapshot } from 'mobx-state-tree';
import { remote } from 'electron';

import ReactTable from "react-table";
import 'react-table/react-table.css'

configure({ enforceActions: true })

const menu = remote.getGlobal('menu');
const MenuItem = remote.getGlobal('MenuItem');

@observer
class MyCell extends React.Component {
  render() {
    return this.props.value;
  }
}


@inject('ADM')
@observer
class DownloadListView extends Component {
  constructor(props) {
    super(props)

    this.downloadStore = this.props.ADM.downloadStore;
    this.createDownloadContextMenu();
  }

  componentDidMount() {
    document.addEventListener("mousedown", (event) => this.handleClick(event), false)
  }

  componentWillUnmount() {
    document.removeEventListener("mousedown", (event) => this.handleClick(event), false)
  }

  @action
  handleClick(event) {
    if (this.node.contains(event.target) || this.props.options.current.contains(event.target)) {

      return;
    }

    this.props.ADM.downloadStore.clearAllSelected();
  }

  createDownloadContextMenu() {
    this.resume = new MenuItem({ label: 'Resume', click: () => { this.downloadStore.resumeSelectedDownload() } })
    this.stop = new MenuItem({ label: 'Stop', click: () => { this.downloadStore.stopDownloads(true) } })
    this.renewUri = new MenuItem({ label: 'Renew URI', click: () => { console.log('no implementation') } })

    menu.append(this.resume)
    menu.append(this.stop)
    menu.append(this.renewUri)
  }

  convertSize(bytes) {
    let size = ``;

    if (bytes / 1000 < 1) {
      // bytes
      size = `${bytes}B`
    }
    else if (bytes / 1000 < 1000) {
      // kB
      size = `${(bytes / 1000).toFixed(3)}KB`
    }
    else if (bytes / 1000 / 1000 < 1000) {
      // mB
      size = `${(bytes / 1000 / 1000).toFixed(3)}MB`
    }
    else {
      // gB
      size = `${(bytes / 1000 / 1000 / 1000).toFixed(3)}GB`
    }

    return size;
  }

  render() {
    const columns = [{
      id: 'FileName',
      Header: 'File Name',
      width: 350,
      accessor: d =>
        d.fileName.replace(/^.*[\\\/]/, '')
    }, {
      id: 'TotalLength',
      Header: 'File Size',
      width: 80,
      accessor: d => this.convertSize(d.totalLength),
    }, {
      id: 'Percentage',
      Header: 'Percentage',
      width: 80,
      accessor: d =>
        `${(d.completedLength / d.totalLength * 100).toFixed(2)}%`,
    }, {
      id: 'DownloadSpeed',
      Header: 'Download',
      width: 80,
      accessor: d => this.convertSize(d.downloadSpeed),
    }, {
      id: 'UploadSpeed',
      Header: 'Upload',
      width: 80,
      accessor: d => this.convertSize(d.uploadSpeed),
    }, {
      accessor: 'state',
      show: false,
    }, {
      accessor: 'id',
      show: false,
    }, {
      accessor: 'selected',
      show: false,
    }, {
      accessor: 'uri',
      show: false,
    }]

    const data = Object.values(getSnapshot(this.props.ADM.downloadStore.downloads));

    return (
      <div ref={node => this.node = node}>
        <ReactTable
          data={data}
          resolveData={data => data.map(row => row)}
          columns={columns}
          defaultPageSize={10}
          noDataText={'No download found'}
          getTrProps={(state, rowInfo, column) => {
            if (rowInfo === undefined) {
              return {};
            }

            let backgroundColor = "";
            let selected = rowInfo.row.selected;

            if (rowInfo.row.state == 'ERROR') {
              backgroundColor = "fdd9d7";
            }

            return {
              style: {
                background: selected ? "#cdeefd" : backgroundColor
              },
              onClick: (e, handleOriginal) => {
                if (!e.ctrlKey && !e.shiftKey) {
                  this.downloadStore.clearAllSelected();
                }

                if (e.shiftKey) {

                }

                this.downloadStore.toggleSelectedRow(rowInfo.row.id);

                if (handleOriginal) {
                  handleOriginal();
                }
              },
              onContextMenu: (e, handleOriginal) => {
                this.downloadStore.contextMenuSelection(rowInfo.row.id);

                let contextState = this.downloadStore.contextMenuOptions;

                this.resume.enabled = contextState.resume;
                this.stop.enabled = contextState.stop;

                menu.popup({});
              }
            };
          }}
        />
      </div>
    );
  }

}

export default DownloadListView
