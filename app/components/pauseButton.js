// Import React
import React from 'react';

// Create Search component class
class PauseButton extends React.Component{
  constructor(props) {
    super(props);
    this.pauseAll = this.pauseAll.bind(this);
  }

  pauseAll() {
    this.props.Aria2Module.pause((err, result) => {console.log(result);});
  }

  render() {
    // Return JSX via render()
    return (
      <button onClick={this.pauseAll}>Pause All</button>
    );
  }

}

// Export Search
export default PauseButton
