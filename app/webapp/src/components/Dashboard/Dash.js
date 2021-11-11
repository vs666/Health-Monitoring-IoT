import React, { Component } from 'react'
import axios from 'axios'
import {
  Button,
  Form,
  Card,
  Jumbotron,
  Table,
  Row,
  Col,
  Container,
} from 'react-bootstrap'
import './Dash.css'
import SimpleCard from './Card'
import Grid from '@material-ui/core/Grid'

export default class Dash extends Component {
  constructor(props) {
    super(props)
    this.state = {
      key: '',
      temp: 98.5,
      blood_pressure: '-',
      gsr: '-',
    }
  }

  login = (event) => {
    event.preventDefault()

    const key_ = this.state.key
    var data = { key: key_.toString() }
    console.log(data)
    // Sending Data
    axios
      .post('http://localhost:5000/getData', data)
      .then((response) => {
        if (response.data['status'] == 'ok') {
          // do all work here :sad
          this.setState({
            temp: response.data['message']['temp'],
            gsr: response.data['message']['gsr'],
            // blood_pressure: response.data['message']['blood_pressure'],
          })
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
        <Grid container justifyContent="center" spacing={24}>
          <Grid item md={3}>
            <SimpleCard
              title="Blood Pressure"
              value={this.state.blood_pressure}
            />
          </Grid>
          <Grid item md={3}>
            <SimpleCard title="Temperature" value={this.state.temp} />
          </Grid>
          <Grid item md={3}>
            <SimpleCard title="Human Resistance" value={this.state.gsr} />
          </Grid>
        </Grid>
        <div
          style={{ margin: '0', position: 'absolute', top: '50%', left: '50%' }}
        >
          <div className="container" style={{ marginTop: '10%' }}>
            <Card style={{ width: '18rem', backgroundColor: '#078' }}>
              <Card.Body>
                <Form
                  style={{ fontSize: '80%' }}
                  method="post"
                  onSubmit={this.login}
                >
                  <Form.Group controlId="formBasicEmail">
                    <Form.Label>Key</Form.Label>
                    <Form.Control
                      validations={['required', 'isEmail']}
                      name="email"
                      id="email"
                      value={this.state.email}
                      onChange={(event) => {
                        this.setState({ key: event.target.value })
                      }}
                      placeholder="Enter Key"
                    />
                  </Form.Group>

                  <Button variant="primary" type="submit">
                    Get Data
                  </Button>
                </Form>
              </Card.Body>
            </Card>
          </div>
        </div>
      </div>
    )
  }
}
