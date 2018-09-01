import React from 'react';
import styled from 'styled-components';

const StyledButton = styled.button`
  font-family: 'Slabo 27px', serif;
  font-size: 15px;
  outline: none;
  height: 40px;
  width: 100px;
  background: #fff;
  border: 1px solid #bdc3c7;
  margin: 8px;
  border-radius: 5px;

  &:hover {
    border: 1px solid black;
  }

  &:active {
    border: 1px solid red;
    color: red;
  }
`;

const Button = ({ children, onClick, innerRef }) => (
  <StyledButton innerRef={innerRef} onClick={onClick}>
    {children}
  </StyledButton>
);

export default Button;
