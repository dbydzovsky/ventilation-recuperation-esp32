import React from 'react';
import {makeStyles} from "@material-ui/core/styles";
import Grid from "@material-ui/core/Grid";
import {useDispatch} from "react-redux";
import {restartESP, resetI2C, resetSensor} from "../actions/actions";
import PowerSettingsNewIcon from '@material-ui/icons/PowerSettingsNew';
import Button from "@material-ui/core/Button";
import {InputLabel, Paper} from "@material-ui/core";
import CallToActionIcon from '@material-ui/icons/CallToAction';
export function ActionPage() {
    const classes = useStyles();
    const dispatch = useDispatch();
    return  <div>
        <Grid container>
            <Grid item xs={12} sm={6} md={6}>
                <Paper className={classes.properties}>
                    <InputLabel htmlFor="standard-adornment-amount" className={classes.label}>
                        <CallToActionIcon/>
                        Ovládací akce
                    </InputLabel>
                    <div className={classes.box}>
                        <Button variant={"contained"}
                                color={"secondary"}
                                startIcon={<PowerSettingsNewIcon/>}
                                onClick={() => {
                                    dispatch(restartESP)
                                }}>
                            Restart zařízení
                        </Button>
                    </div>
                    <div className={classes.box}>
                        <Button variant={"contained"}
                                color={"secondary"}
                                startIcon={<PowerSettingsNewIcon/>}
                                onClick={() => {
                                    dispatch(resetI2C)
                                }}>
                            Restart I2C communication
                        </Button>
                    </div>
                    <div className={classes.box}>
                        <Button variant={"contained"}
                                color={"secondary"}
                                startIcon={<PowerSettingsNewIcon/>}
                                onClick={() => {
                                    dispatch(resetSensor)
                                }}>
                            Restart sensor power
                        </Button>
                    </div>
                </Paper>
            </Grid>
        </Grid>
    </div>
}

const useStyles = makeStyles(() => {
    return {
        label: {
            display: "flex",
            alignItems: "center",
        },
        box: {
            margin: 20
        },
        properties: {
            margin: 20,
            padding: 20
        },
        root: {
            margin: 20
        },
        row: {
            textAlign: "center"
        }
    }
})