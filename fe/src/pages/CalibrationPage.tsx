import React, {useEffect} from 'react'
import { makeStyles } from '@material-ui/core/styles';
import {
    Button,
    Input,
    InputAdornment,
    InputLabel,
    Paper
} from "@material-ui/core";
import {useActions} from "../actions";
import * as CalibrationActions from "../actions/calibration";
import {useSelector} from "react-redux";
import {RootState} from "../reducers";
import Grid from "@material-ui/core/Grid";
import {CalibrationData} from "../actions/calibration";
import WhatshotIcon from '@material-ui/icons/Whatshot';

export function CalibrationPage() {
    const classes = useStyles();
    const calibrationAction = useActions(CalibrationActions);
    const currentState = useSelector((state: RootState) => state.state);
    const [calibration, setCalibration] = React.useState(Math.round(currentState.i.t * 10)/10);
    const calibrate = () => {
        const data : CalibrationData = {
            insideTemp: calibration
        }
        calibrationAction.calibrate(data)
    }
    return <div className={classes.header}>
        <Grid container>
            <Grid item xs={12} sm={6} md={6}>
                <Paper className={classes.properties}>
                    <InputLabel htmlFor="standard-adornment-amount" className={classes.label}>
                        <WhatshotIcon/>
                        Kalibrace vnitřního čidla teploty: Kolik je teď stupňů?
                    </InputLabel>
                    <Input
                        id="standard-adornment-calibration"
                        value={calibration}
                        type={"number"}
                        className={classes.input}
                        onChange={(event) => {
                            const value = event.target.value;
                            setCalibration(Math.round(Number(value) * 10)/10)
                        }}
                        startAdornment={<InputAdornment position="start">°C</InputAdornment>}
                    />
                    <Button variant="contained" color="secondary" className={classes.btn} onClick={calibrate}>Kalibrovat</Button>
                </Paper>
            </Grid>
        </Grid>
    </div>
}


const useStyles = makeStyles({
    label: {
        display: "flex",
        alignItems: "center",
    },
    helplabel: {
        fontSize: "80%",
        fontStyle: "italic"
    },
    radioBtnGroup:{
        display: "block"
    },
    properties: {
        margin: 20,
        padding: 20
    },
    header: {
    },
    input: {
        margin: 20
    },
    btn: {
        margin: 20
    },
})