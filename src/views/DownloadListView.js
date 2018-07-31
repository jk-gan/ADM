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
    }]

    const data = Object.values(getSnapshot(this.props.ADM.downloadStore.downloads));

    return (
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

          return {
            style: {
              background: rowInfo.row.state == 'ERROR' ? "#fdd9d7" : ""
            }
          };
        }}
      />
    );
  }

}

export default DownloadListView
