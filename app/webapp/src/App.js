import logo from './logo.svg'
import React, { Component } from 'react'
import { Route, BrowserRouter, Switch } from 'react-router-dom'
import './App.css'
import Upd from './components/Update/Upd'

// import SignUp from './Components/SignUp/SignUp';
import Dash from './components/Dashboard/Dash'
// import Login from './Components/LoginPage/Login';
import Navigation from './components/NavB/Navigation'
// import FSign from './Components/SignUp/FSign';
// import Logout from './Components/LoginPage/Logout';

class App extends Component {
  constructor(props) {
    super(props)
    this.state = {
      data: null,
    }
  }
  
  componentDidMount() {
    // Call our fetch function below once the component mounts
    this.callBackendAPI()
      .then((res) => this.setState({ data: res.express }))
      .catch((err) => console.log(err))
    console.log(this.state.data)
  }

  callBackendAPI = async () => {
    const response = await fetch('localhost:5000/')
    const body = await response.json()

    if (response.status !== 200) {
      console.log('Server is down.')
      throw Error(body.message)
    } else {
      console.log('Server is up.')
    }
    return body
  }

  logoutFunc = () => {
    // localStorage.removeItem('hash');
    localStorage.removeItem('hash')
  }

  render() {
    return (
      <div className="App">
        <Navigation />
        <header className="App-header">
          <BrowserRouter>
            <Route exact path="/"></Route>
            <Route path="/dashboard">
              <Dash />
              </Route>
            <Route path="/updateInfo">
              <Upd />
            </Route>
          </BrowserRouter>
        </header>
      </div>
    )
  }
}

export default App
