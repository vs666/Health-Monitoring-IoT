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
} from 'react-bootstrap'
import './Dash.css'
import SimpleCard from './Card'
import Grid from '@material-ui/core/Grid'

export default class Dash extends Component {
  constructor(props) {
    super(props)
    this.state = {
      key: '',
      temp: '',
      bp_dia : '',
      bp_sys : '',
      gsr: '-',
      spo2:'',
      heart_rate:'',
      covid_stat:false,
    }
    this.interval = setInterval(() => {if(this.state.key!=''){this.login()}}, (1000 * 5))
    //destroy interval on unmount
    
  }
  componentWillUnmount(){
    return () => clearInterval(this.interval)
  }  
  getMsg_spo2=()=>{
    if(this.state.covid_stat){
      return "Our ML predicts chance of Covid. Please refer to medical assistance immediately."
    }
    else {
      return "ML says all good. Carry on."
    }
  }
 

  getMsg_panicAttacks = ()=>{
    if((parseInt(this.state.bp_sys) >= 140.0 && this.state.bp_dia >= 90) && (parseInt(this.state.heart_rate) >= 120)){
      return "This is an alert for a panic attack and/or a possible pre-emptive warning for a cardiac arrest. Kindly seek medical help immediately."
    }
    else {
      return "All good. Carry on."
    }
  }

  login = (event) => {
    if(event)
    {    
      event.preventDefault()
    }
    const key_ = this.state.key
    var data = { key: key_.toString() }
    console.log(data)
    // Sending Data
    axios
      .post('http://localhost:5000/getData', data)
      .then((response) => {
        if (response.data['status'] == 'ok') {
          // do all work here :sad
          var elt = JSON.parse(response.data['message']);
          console.log('rec : ',elt);
          this.setState({
            temp: elt['temp'],
            gsr: elt['hr'],
            bp_dia: elt['bpdia'],
            bp_sys: elt['bpsys'],
            spo2:elt['spo2'],
            heart_rate:elt['hrate'],
            covid_stat: response.data['covidstat']==0?false:true,
          })
          console.log(response);
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
              value={this.state.bp_sys+"/"+this.state.bp_dia}
            />
          </Grid>
          <Grid item md={3}>
            <SimpleCard title="Temperature" value={this.state.temp} />
          </Grid>
          <Grid item md={3}>
            <SimpleCard title="Human Resistance" value={this.state.gsr} />
          </Grid>
          <Grid item md={3}>
            <SimpleCard title="spO2" value={this.state.spo2} />
          </Grid>
          <Grid item md={3}>
            <SimpleCard title="Heart-Rate" value={this.state.heart_rate} />
          </Grid>

        </Grid>
          <Row>
            <Col>
          <Alert variant="warning" style={{marginLeft:'5vh'}}>
            <Alert.Heading>COVID ALERT</Alert.Heading>
            <p>
              {this.getMsg_spo2()}
            </p>
            <hr />
            <p className="mb-0">
              Covid Symptoms : High-Fever, Low levels of SPO2, Cough. If such symptoms occur, consult a doctor immediately.
            </p>
          </Alert>
          </Col>
          <Col>
          <Alert variant="danger" style={{paddingLeft:'5vh'}}>
            <Alert.Heading>Panic Attack Alert</Alert.Heading>
            <p>
              {this.getMsg_panicAttacks()}              
            </p>
            <hr />
            <p className="mb-0">
              Panic Attack : Stress and anxiety, high blood pressure, heart rate, and/or respiratory symptoms. If such symptoms occur, consult a doctor immediately.
            </p>
          </Alert>
            </Col>
          </Row>
        <div>
          <iframe width={"450"} height={"260"} style={{border: '1px solid #cccccc;', margin:'5vh'}} src={"https://thingspeak.com/channels/1599973/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=BodyTemperature&type=line"}></iframe>
          <iframe width={"450"} height={"260"} style={{border: '1px solid #cccccc;', margin:'5vh'}} src={"https://thingspeak.com/channels/1599973/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=HumanResistance&type=line"}></iframe>
          <br></br>
          <iframe width={"450"} height={"260"} style={{border: '1px solid #cccccc;', margin:'5vh'}} src={"https://thingspeak.com/channels/1599973/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=BP%28sys%29&type=line"}></iframe>
          <iframe width={"450"} height={"260"} style={{border: '1px solid #cccccc;', margin:'5vh'}} src={"https://thingspeak.com/channels/1599973/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=BP%28dia%29e&type=line"}></iframe>
            <br></br>
          <iframe width={"450"} height={"260"} style={{border: '1px solid #cccccc;', margin:'5vh'}} src={"https://thingspeak.com/channels/1599973/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=HeartRate&type=line"}></iframe>
          <iframe width={"450"} height={"260"} style={{border: '1px solid #cccccc;', margin:'5vh'}} src={"https://thingspeak.com/channels/1599973/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=SPO2&type=line"}></iframe>
        </div>
        <div style={{ margin: '0', top: '50%', left: '50%' }}>
          <div className="container" style={{ marginTop: '2vh' }}>
            <Card style={{ width: '18rem',color:'white',width:'bolder', backgroundColor: 'rgba(20,20,20,0.3)' }}>
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

                  <Button variant="success" type="submit">
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
