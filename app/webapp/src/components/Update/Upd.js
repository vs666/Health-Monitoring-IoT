import React, { Component } from 'react'
import axios from 'axios'
import Alert from 'react-bootstrap/Alert'
import ProgressBar from 'react-bootstrap/ProgressBar'
import {
  Button,
  Form,
  Card,
  Jumbotron,
  Table,
  Row,
  Col,
  Container,
  Dropdown,
  DropdownButton,

} from 'react-bootstrap'

import './Upd.css'
import SimpleCard from './Card'
import Grid from '@material-ui/core/Grid'

export default class Upd extends Component {
  constructor(props) {
    super(props)
    this.state = {
      gender: '1',
      age: '20',
      BMI : '24.0',
      BSA : '0.0',
    }
    // this.interval = setInterval(() => {if(this.state.key!=''){this.login()}}, (1000 * 5))
    //destroy interval on unmount
    
  }

  setGender = (event)=> {
    event.preventDefault()
    this.setState({
      gender:event.target.value
    });
    console.log(this.state)
  }

  setAge = (event)=> {
    event.preventDefault()
    this.setState({
      age:event.target.value
    });
    console.log(this.state)
  }

  setBMI = (event)=> {
    event.preventDefault()
    this.setState({
      BMI:event.target.value
    });
    console.log(this.state)
  }


  handleSubmit = (event) => {
    if(event)
    {    
      event.preventDefault()
    }
    // Sending Data
    axios
      .post('http://localhost:5000/updateInfo', this.state)
      .then((response) => {
        if (response.data['status'] == 'ok') {
          alert('Data Updated.');
        } else {
          console.log(response.data)
        }
      })
      .catch((error) => {
        console.log(error, 'Error')
      })
  }

  render() {
    return (
      <div style={{ paddingTop: '10%', paddingRight: '2vh' }}>

      <form onSubmit={this.handleSubmit}>
        <label>
          Gender 
          <select style={{marginLeft:'5vh'}} value={this.state.gender} onChange={this.setGender}>
            <option value="1">Female</option>
            <option value="0">Male</option>
          </select>
        </label>
        <br></br>
        <label>
          Age:
          <input style={{marginLeft:'5vh'}} type="number" name="age" value={this.state.age} onChange={this.setAge} />
        </label>

        <br></br>
        <label>
          BMI:
          <input style={{marginLeft:'5vh'}} type="number" name="BMI" value={this.state.BMI} onChange={this.setBMI}/>
        </label>

        <br></br>
        <input type="submit" value="Submit" />
      </form>
      </div>
    );
    
  }
}
