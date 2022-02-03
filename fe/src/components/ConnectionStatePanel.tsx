import React from 'react';
import CheckCircleOutlineIcon from '@material-ui/icons/CheckCircleOutline';
import {useSelector} from "react-redux";
import {RootState} from "../reducers";
import SignalCellularConnectedNoInternet0BarIcon from '@material-ui/icons/SignalCellularConnectedNoInternet0Bar';
import {makeStyles} from "@material-ui/core/styles";
import HourglassEmptyIcon from '@material-ui/icons/HourglassEmpty';
import {ConnectionState} from "../model/state";

export function ConnectionStatePanel() {
    const errors = useSelector((state: RootState) => state.errorState);
    const classes = useStyles();
    if (errors.state == ConnectionState.Connected) {
        return <div className={classes.alignment + " " + classes.on}>
            <CheckCircleOutlineIcon/> Online
        </div>
    } else if (errors.state == ConnectionState.Disconnected){
        return <div className={classes.alignment + " " + classes.off}>
            <SignalCellularConnectedNoInternet0BarIcon/> Offline
        </div>
    } else {
        return <div className={classes.alignment + " " + classes.loading}>
            <HourglassEmptyIcon/> Loading..
        </div>
    }
}

const useStyles = makeStyles({
    alignment: {
        display: "flex",
        alignItems: "center",
        justifyContent: "right",
        fontSize: "80%",
    },
    on: {
        color: "green",
    },
    off: {
        color: "orange",
    },
    loading: {
        color: "blue",
    }
})