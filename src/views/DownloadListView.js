import React, { Component } from 'react';
import { observer, inject } from 'mobx-react'
import { configure, action, observable } from 'mobx';
import { getSnapshot } from 'mobx-state-tree';

import ReactTable from "react-table";
import 'react-table/react-table.css'

configure({ enforceActions: true })

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
                if (!e.ctrlKey) {
                  this.props.ADM.downloadStore.clearAllSelected();
                }

                if (e.shiftKey) {
                  this.props.ADM.downloadStore.resumeDownload(rowInfo.row.id, "http://103.1.138.206/files2.codecguide.com/K-Lite_Codec_Pack_1425_Mega.exe");
                }

                this.props.ADM.downloadStore.toggleSelectedRow(rowInfo.row.id);

                if (handleOriginal) {
                  handleOriginal();
                }
              }
            };
          }}
        />
      </div>
    );
  }

}

export default DownloadListView
