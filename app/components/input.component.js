// Import React
import React from 'react';

// Create Search component class
class Input extends React.Component{

  render() {
    // Return JSX via render()
    return (
      <form>
        <input type="text" placeholder="New Download Link" />
        <button type="submit">Download</button>
      </form>
    );
  }

}

// Export Search
export default Input
