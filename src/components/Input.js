// Import React
import React from 'react'
import styled from 'styled-components'

const StyledInput = styled.input`
  height: 50px;
  font-size: 20px;
  width: 100%;
`

// Create Search component class
const Input = () => (
  <StyledInput type="text" placeholder="e.g. www.abc.com" />
)

// Export Search
export default Input
