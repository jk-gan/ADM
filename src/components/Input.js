// @flow

import React, {Component} from 'react';
import {configure} from 'mobx';
import {observer} from 'mobx-react';
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

configure({enforceActions: true});

type Props = {
  type: string,
  placeholder: string,
  newLink: string,
  onChange: Function,
  onKeyUp: Function,
};

@observer
class Input extends Component {
  props: Props;

  render() {
    const {type, placeholder, newLink, onChange, onKeyUp} = this.props;

    return (
      <StyledInput
        type={type}
        placeholder={placeholder}
        value={newLink}
        onChange={onChange}
        onKeyUp={onKeyUp}
      />
    );
  }
}

export default Input;
