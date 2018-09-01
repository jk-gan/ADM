// Import React
import React from 'react';

class PauseAllButton extends React.Component {
  constructor(props) {
    super(props);
    this.pauseAll = this.pauseAll.bind(this);
  }

  pauseAll() {
    this.props.Aria2Module.pause(-9999, (err, result) => {
      console.log(result);
    });
  }

  render() {
    // Return JSX via render()
    return <button onClick={this.pauseAll}>Pause All</button>;
  }
}

export default PauseAllButton;
