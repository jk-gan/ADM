// ES6 Component
// Import React and ReactDOM
import React from 'react';
import ReactDOM from 'react-dom';

import Input from './components/Input'
import DownloadList from './components/DownloadList'

class App extends React.Component {
  constructor() {
    super();
    this.addDownloadLink = this.addDownloadLink.bind(this);
    this.state = {
      downloadLinks: {}
    };
  }

  addDownloadLink(downloadLink) {
    const downloadLinks = {...this.state.downloadLinks};
    const timestamp = Date.now();
    downloadLinks[`downloadLink-${timestamp}`] = downloadLink;
    this.setState({ downloadLinks });
  }

    // render method is most important
    // render method returns JSX template
    render() {
        return (
          <div>
            <Input />
            <DownloadList />
          </div>
        );
    }
}

// Render to ID content in the DOM
ReactDOM.render( <App /> ,
    document.getElementById('content')
);
