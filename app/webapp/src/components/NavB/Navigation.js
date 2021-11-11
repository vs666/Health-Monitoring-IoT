import React from 'react'
import { Nav, Navbar, NavDropdown, Card } from 'react-bootstrap'
import styled from 'styled-components'
import './Nav.css'

const Styles = styled.div`
  .navbar {
    background-color: rgb(20, 20, 4 0);
  }

  .navbar-brand,
  .navbar-nav,
  .nav-link {
    color: #bbb;
    $: hover {
      color: white;
    }
  }
`

const Navigation = () => {
  return (
    <Styles>
      <Navbar
        raised={true}
        expand="lg"
        className="navbar a"
        fixed="top"
        style={{ backgroundColor: 'rgba(0,0,0,0.3)' }}
      >
        {/* <Navbar.Brand href="/"><img src={require("")} alt="PC-Cart" height="64" width="64" /></Navbar.Brand> */}
        <h2 style={{ padding: '1vh' }}>Wearable-Device</h2>
        <Navbar.Toggle aria-controls="basic-navbar-nav" />
        <Navbar.Collapse id="basic-navbar -nav" className="navbar a">
          <Nav className="mr-auto" style={{ color: '#fff' }}>
            <Nav.Link href="/" style={{ color: '#000' }}>
              Home
            </Nav.Link>
          </Nav>
          <Nav className="" style={{ color: '#fff' }}>
            <Nav.Link href="/dashboard" style={{ color: '#000' }}>
              Dashboard
            </Nav.Link>
          </Nav>
        </Navbar.Collapse>
      </Navbar>
    </Styles>
  )
}

export default Navigation
