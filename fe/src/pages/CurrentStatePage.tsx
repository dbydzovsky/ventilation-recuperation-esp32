import React from 'react'
import {makeStyles, withStyles} from '@material-ui/core/styles';
import LinearProgress from '@material-ui/core/LinearProgress';
import {CurrentStatePanel} from "./CurrentStatePanel";
import {TempHumSensorComp} from "../components/TempHumSensor";
import {TempCo2SensorComp} from "../components/TempPressSensor";
import {useSelector} from "react-redux";
import {RootState} from "../reducers";
import Grid from "@material-ui/core/Grid";
import {Paper, Table} from "@material-ui/core";

const BorderLinearProgress = withStyles((theme) => ({
    root: {
        height: 10,
        borderRadius: 5,
    },
    colorPrimary: {
        backgroundColor: theme.palette.grey[theme.palette.type === 'light' ? 200 : 700],
    },
    bar: {
        borderRadius: 5,
        backgroundColor: '#1a90ff',
    },
}))(LinearProgress);

export function CurrentStatePage() {
    const classes = useStyles();
    const currentState = useSelector((state: RootState) => state.state);

    return <div className={classes.root}>
        <CurrentStatePanel/>
        <Grid container>
            <Grid item xs={12} md={4} lg={4} className={classes.sensor}>
                <Paper className={classes.sensorInside}>
                    <TempHumSensorComp hum={currentState.outsideHum || {}} temp={currentState.outsideTemp|| {}} label={"Venkovní čidlo"} />
                </Paper>
            </Grid>
            <Grid item xs={12} md={4} lg={4} className={classes.sensor}>
                <Paper className={classes.sensorInside}>
                    <TempCo2SensorComp temp={currentState.insideTemp|| {}} co2={currentState.co2Inside|| {}} label={"Vnitřní čidlo"} />
                </Paper>
            </Grid>
            <Grid item xs={12} md={4} lg={4} className={classes.sensor}>
                <Paper className={classes.sensorInside}>
                    <div className={classes.margination}>
                        <b>Zapnuto</b><br/> {msToTime(currentState.alive)}
                    </div>
                </Paper>
            </Grid>
        </Grid>
    </div>
}


const useStyles = makeStyles({
    root: {
        margin: 20
    },
    sensor: {
        padding: 10,
    },
    sensorInside: {
        padding: 5
    },
    margination: {
        margin: 10
    }
})


function msToTime(s: number): string {
    if (s){
        var ms = s % 1000;
        s = (s - ms) / 1000;
        var secs = s % 60;
        s = (s - secs) / 60;
        var mins = s % 60;
        var hrsRaw = (s - mins) / 60;
        var hrs = (hrsRaw) % 24;
        var days = (hrsRaw-hrs) / 24;

        let result = "";
        if (days == 0) {

        } else if (days == 1) {
            result += days + " den, "
        } else if (days >= 2 && days < 5) {
            result += days + " dny, "
        } else {
            result += days + " dní, "
        }

        if (hrs == 0) {

        } else if (hrs == 1) {
            result += hrs + " hodina, "
        } else if (hrs >= 2 && hrs < 5) {
            result += hrs + " hodiny, "
        } else {
            result += hrs + " hodin, "
        }
        return result + mins + ' minut a ' + secs + ' sekund';
    } else {
        return "0s";
    }
}

