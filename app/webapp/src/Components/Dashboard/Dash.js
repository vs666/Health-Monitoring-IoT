import React, { Component } from 'react';
import axios from 'axios';
import { Button, Form, Card, Jumbotron, Table, Row, Col } from 'react-bootstrap';
import './Dash.css';

export default class Dash extends Component {

    constructor(props) {
        super(props);
        this.state = {
            key:'',
            temp:98.5,
            blood_pressure:'-',
            gsr:'-'
        }
    }

    login = (event) => {

        event.preventDefault();

        const key_ = this.state.key;
        var data = { key: key_.toString()};
        console.log(data);
        // Sending Data
        axios.post('http://localhost:5000/getData', data)
                .then((response) => {
                    if (response.data['status'] == "ok") {
                        // do all work here :sad
                        this.setState({
                            temp: response.data['message']['temp'],
                            gsr: response.data['message']['gsr'],
                            // blood_pressure: response.data['message']['blood_pressure'],
                        });
                    }
                    else {
                        console.log(response.data);
                    }
                })
                .catch((error) => {
                    console.log(error, 'Error');
                });
    }

    render() {
            return (
                <div style={{ paddingTop: '10%', paddingRight: '2vh' }}>
                    {/* <p className="top-tag" style={{ textAlign: 'right' }}>{this.state.username}</p> */}
                    <div>
                        <Jumbotron style={{ marginLeft: '5vh', marginRight: '5vh', color: 'black', backgroundColor: 'rgba(190,190,255,0.5)' }}>
                            <Col>
                                <Table>
                                    <Row>
                                        <Col>Temperature</Col>
                                        <Col>{this.state.temp}</Col>
                                    </Row>
                                    <Row>
                                        <Col>GSR</Col>
                                        <Col>{this.state.gsr}</Col>
                                    </Row>
                                    <Row>
                                        <Col>Blood Pressure</Col>
                                        <Col>{this.state.blood_pressure}</Col>
                                    </Row>
                                </Table>
                            </Col>
                        </Jumbotron>

                    </div>
                    <div style={{  margin: '0',
                      position: 'absolute',
                      top: '50%',
                      left: '50%',}}>    
                        <div className="container" style={{ marginTop: '10%'}}>
                            <Card style={{ width: '18rem', backgroundColor: '#078' }}>
                                <Card.Body>
                                    <Form style={{ fontSize: '80%' }} method="post" onSubmit={this.login}>
                                        <Form.Group controlId="formBasicEmail" >
                                            <Form.Label>Key</Form.Label>
                                            <Form.Control type="email" validations={['required', 'isEmail']} name="email" id="email" value={this.state.email} onChange={
                                                (event) => {
                                                    this.setState({ key: event.target.value });
                                                }
                                            } placeholder="Enter Key" />
                                        </Form.Group>

                                        <Button variant="primary" type="submit" >
                                            Get Data
                                        </Button>
                                    </Form>
                                </Card.Body>
                            </Card>
                        </div>
                    </div>
                </div>

            );
    }
}
