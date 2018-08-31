import React, { Component } from 'react';
import { configure, action, observable } from 'mobx';
import { observer } from 'mobx-react';
import styled from 'styled-components';

const StyledInput = styled.input`
  font-family: 'Slabo 27px', serif;
  outline: none;
  height: 50px;
  font-size: 20px;
  width: 90%;
  padding: 0px 15px;
  border: 1px solid #bdc3c7;
  border-radius: 5px;

  &:focus {
    border: 1px solid #3498db;
  }
`;

configure({ enforceActions: true });

@observer
class Input extends Component {
  render() {
    return (
      <StyledInput
        type="text"
        placeholder="e.g. www.abc.com"
        value={this.props.newLink}
        onChange={this.props.onChange}
        onKeyUp={this.props.onKeyUp}
      />
    );
  }
}

export default Input;
