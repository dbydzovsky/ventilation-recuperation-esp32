import React, {useEffect} from 'react'
import ToysIcon from '@material-ui/icons/Toys';
import {makeStyles, withStyles} from '@material-ui/core/styles';
import LinearProgress from '@material-ui/core/LinearProgress';
import {useDispatch, useSelector} from "react-redux";
import {RootState} from "../reducers";
import {ConnectionStatePanel} from "../components/ConnectionStatePanel";
import NotInterestedIcon from '@material-ui/icons/NotInterested';
import {useActions} from "../actions";
import * as StateActions from "../actions/state";
import {ConnectionState} from "../model/state";
import {Mode} from "../model/configuration";
import Grid from "@material-ui/core/Grid";
import WarningIcon from "@material-ui/icons/Warning";

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

export function CurrentStateRefresher() {
    const stateAction = useActions(StateActions);
    useEffect(() => {
        let isSubscribed = true;
        let schedule = () => {
            stateAction.getState({
                onDone: () => {
                    setTimeout(() => {
                        if (isSubscribed) schedule();
                    }, 2000);
                }
            })
        }
        schedule();
        return () => {
            isSubscribed = false
        }
    }, []);
    return <div></div>
}

export function CurrentStatePanel() {
    const classes = useStyles();

    const dispatch = useDispatch();
    const currentState = useSelector((state: RootState) => state.state);
    const cleaningNeeded = currentState.filterRecuperation.needCleaning || currentState.filterVentilator.needCleaning;
    const connectionState = useSelector((state: RootState) => state.errorState).state;
    let clazz = classes.connected;
    if (connectionState == ConnectionState.Disconnected) {
        clazz = classes.disconnected;
    } else if (connectionState == ConnectionState.TooMuchRequests) {
        clazz = classes.loading;
    } else if (connectionState == ConnectionState.Unitiliazed) {
        clazz = classes.unitialized;
    }
    let recuperationPower = currentState.recuperation === undefined ? 0 : currentState.recuperation
    return <div className={classes.header}>
        {cleaningNeeded && <p style={{textAlign: "center", color: "orange"}}><WarningIcon htmlColor={"orange"}/>Je třeba vyčistit filtr</p>}
        <ConnectionStatePanel/>
        { connectionState == ConnectionState.Unitiliazed && "Loading.." }
        { connectionState != ConnectionState.Unitiliazed && currentState.mode == Mode.INACTIVE && <div className={classes.turnedOff}><NotInterestedIcon/> Neaktivní (vypnuto)</div>}
        <div><b>{currentState.description}</b></div>
        <div className={clazz}>
            <VentilatorIcon
                highRPM={currentState.alarmVentilator.highRpm}
                blocked={currentState.alarmVentilator.blocked}
                percentage={currentState.ventilator} />
            Ventilátor ({currentState.ventilatorRPM} RPM) {currentState.ventilator}%
        </div>
        <BorderLinearProgress variant="determinate" className={classes.progress} value={currentState.ventilator} />
        <div className={clazz}>
            <VentilatorIcon
                highRPM={currentState.alarmRecuperation.highRpm}
                blocked={currentState.alarmRecuperation.blocked}
                percentage={recuperationPower} />
            Rekuperace ({currentState.recuperationRPM} RPM) {recuperationPower}%
        </div>
        <Grid container>
            <Grid item xs={6} >
                <BorderLinearProgress variant="determinate"
                                      className={classes.progress+ " " + classes.rotated}
                                      value={Math.abs(Math.min(recuperationPower, 0))} />
            </Grid>
            <Grid item xs={6}>
                <BorderLinearProgress variant="determinate"
                                      className={classes.progress}
                                      value={Math.max(recuperationPower, 0)} />
            </Grid>
        </Grid>
        {currentState.time}
    </div>
}

export function VentilatorIcon(props: {percentage: number, highRPM?: boolean, blocked?: boolean, disabled?: boolean}) {
    let style: any = {}
    if (props.disabled) {
        style["color"] = "grey"
    }
    if (props.blocked) {
        return <ToysIcon className={"blocked"}/>
    }
    if (props.highRPM) {
        return <ToysIcon className={"highRPM"}/>
    }
    if (props.percentage) {
        if (props.percentage > 0) {
            style["animationDuration"]= (10/props.percentage * 5000)+"ms";
            return <ToysIcon className={"spin"} style={style}/>
        } else {
            style["animationDuration"]= (10/Math.abs(props.percentage) * 5000)+"ms";
            return <ToysIcon className={"spinCounter"} style={style}/>
        }
    } else {
        return <ToysIcon style={style}/>
    }
}


const useStyles = makeStyles({
    header: {
        textAlign: "center",
        margin: 10,
    },
    flex: {
    },
    turnedOff: {
        display: "flex",
        alignItems: "center",
        justifyContent: "center",
        color: "red"
    },
    progress: {
        marginTop: 5,
        marginBottom: 15,
    },
    rotated:{
        transform: "rotate(180deg)",
    },
    disconnected: {
        color: "grey"
    },
    unitialized: {
        color: "orange"
    },
    loading: {
        color: "blue"
    },
    connected: {

    }
})