// Import React
import React from 'react';

class PauseButton extends React.Component{
  constructor(props) {
    super(props);
    this.pause = this.pause.bind(this);
  }

  pause() {
    this.props.Aria2Module.pause((err, result) => {console.log(result);});
  }

  render() {
    // Return JSX via render()
    return (
      <button onClick={this.pause} ref={this.downloadId = 123}>Pause</button>
    );
  }
}

// Export Search
export default PauseButton
