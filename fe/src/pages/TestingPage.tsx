import React from 'react';
import {makeStyles} from "@material-ui/core/styles";
import Grid from "@material-ui/core/Grid";
import {useDispatch, useSelector} from "react-redux";
import PowerSettingsNewIcon from '@material-ui/icons/PowerSettingsNew';
import PhotoFilterIcon from '@material-ui/icons/PhotoFilter';
import WarningIcon from "@material-ui/icons/Warning";
import Button from "@material-ui/core/Button";
import {Container, InputLabel, Paper} from "@material-ui/core";
import {cleanFilter, FanType} from "../actions/filter";
import {RootState} from "../reducers";
import {CurrentState, FilterState, Sensor} from "../model/state";
import {setTestingSensors, TestingData} from "../actions/testing";
import Slider from "@material-ui/core/Slider";
import Typography from "@material-ui/core/Typography";
import Box from "@material-ui/core/Box";

export function TestingPage() {
    const classes = useStyles();
    const actual = useSelector((state: RootState) => state.state);
    const dispatch = useDispatch();
    const onChange = (data: TestingData) => {
        dispatch(setTestingSensors(data))
    }

    return <Container>
        <Item onChange={(humIn) => onChange({humIn})}
              title={"Vnitřní vlhkost"}
              min={0}
              max={100}
              t={"%"}
              sensor={actual.insideHum}/>
        <Item onChange={(tempIn) => onChange({tempIn})}
              title={"Vnitřní teplota"}
              min={15}
              max={25}
              t={"°C"}
              sensor={actual.insideTemp}/>
        <Item onChange={(humOut) => onChange({humOut})}
              title={"Venkovní vlhkost"}
              min={0}
              max={100}
              t={"%"}
              sensor={actual.outsideHum}/>
        <Item onChange={(tempOut) => onChange({tempOut})}
              title={"Venkovní teplota"}
              min={-50}
              max={130}
              t={"°C"}
              sensor={actual.outsideTemp}/>
        <Item onChange={(co2) => onChange({co2})}
              title={"CO2"}
              min={0}
              max={5000}
              t={"PPM"}
              sensor={actual.co2Inside}/>
    </Container>
}

interface ItemProps {
    onChange: (val: number) => void
    min: number
    max: number
    sensor: Sensor | undefined
    title: string
    t: string
}
function Item (props: ItemProps){
    const sensor = props.sensor;
    if (!sensor) {
        return null;
    }
    return <>
        <Box margin={2}>
            <Typography gutterBottom>
                {props.title} [{sensor.avg}{props.t}]
            </Typography>
            <Slider defaultValue={sensor.v}
                marks={[{
                    value: props.min,
                    label: props.min + props.t
                },{
                    value: props.max,
                    label: props.max + props.t
                }]}
                onChangeCommitted={(e,x) => props.onChange(Number(x))}
                min={props.min}
                max={props.max}
                    color={"secondary"}
                aria-label="Default"
                valueLabelDisplay="auto" />
        </Box>

    </>
}

const useStyles = makeStyles(() => {
    return {

    }
})