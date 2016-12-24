// Import React
import React from 'react';

// Create Search component class
class Input extends React.Component{
  startDownload(event) {
    event.preventDefault();
    const newLink = this.newLink.value;
    console.log(newLink);
  }

  render() {
    // Return JSX via render()
    return (
      <form onSubmit={ (e) => this.startDownload(e) }>
        <input type="text" placeholder="New Download Link" ref={ (input) => this.newLink = input } />
        <button type="submit">Download</button>
      </form>
    );
  }

}

// Export Search
export default Input
