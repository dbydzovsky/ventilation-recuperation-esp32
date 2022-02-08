import React from 'react';
import {makeStyles} from "@material-ui/core/styles";
import Grid from "@material-ui/core/Grid";
import {useDispatch, useSelector} from "react-redux";
import PowerSettingsNewIcon from '@material-ui/icons/PowerSettingsNew';
import PhotoFilterIcon from '@material-ui/icons/PhotoFilter';
import WarningIcon from "@material-ui/icons/Warning";
import Button from "@material-ui/core/Button";
import {InputLabel, Paper} from "@material-ui/core";
import {cleanFilter, FanType} from "../actions/filter";
import {RootState} from "../reducers";
import {AlarmState, FilterState} from "../model/state";
import {msToTime} from "./CurrentStatePage";
import {cleanAlarm} from "../actions/alarm";
import AlarmIcon from '@material-ui/icons/Alarm';
export function AlarmPage() {
    const ventilatorAlarm = useSelector((state: RootState) => state.state.alarmVentilator);
    const recuperationAlarm = useSelector((state: RootState) => state.state.alarmRecuperation);

    return <Grid container>
            <AlarmShowcase filter={FanType.ventilator}
                            state={ventilatorAlarm}
                            title={"Alarm ventilátoru"}
            />
            <AlarmShowcase filter={FanType.recuperation}
                            state={recuperationAlarm}
                            title={"Alarm rekuperace"}
            />
        </Grid>
}

export interface AlarmShowcaseProps {
    filter: FanType
    state: AlarmState
    title: string
}
export function AlarmShowcase(props: AlarmShowcaseProps)  {
    const classes = useStyles();
    const dispatch = useDispatch();

    return <Grid item xs={12} sm={12} md={6}>
        <Paper className={classes.properties}>
            <InputLabel htmlFor="standard-adornment-amount" className={classes.label}>
                <AlarmIcon/>
                {props.title}
            </InputLabel>
            {!props.state.needAttention && <div>
                <p>
                    Nebyla zaznamenána žádná závada.
                </p>
            </div>}
            {props.state.needAttention && <div style={{color: "orange"}}>
                <p>
                    <WarningIcon htmlColor={"orange"}/> Byl spuštěn alarm, neboť motor se nechová dle očekávání.
                    Pro nápravu je důležité motor zkontrolovat a poté alarm uvolnit stisknutím tlačítka.
                </p>
            </div>}
            <p>
                {props.state.highRpm && <>
                    Motor se točil rychleji, než jsou jeho možnosti. Pravděpodobně větrné počasí.
                    Motor byl dočasně odstaven. Bude automaticky uvolněn za {props.state.remainMinutes} minut.
                </>}
                {props.state.blocked && <>
                    Větrák je zablokovaný nebo se neotáčí v okamžiku, když by měl.
                    K uvolnění alarmu dojde automaticky za {props.state.remainMinutes} minut.
                </>}
            </p>
            {props.state.needAttention && <div className={classes.box}>
                <Button variant={props.state.needAttention ? "contained" : "outlined"}
                        color={props.state.needAttention ? "secondary" : "primary"}
                        disabled={!props.state.needAttention}
                        startIcon={<PowerSettingsNewIcon/>}
                        onClick={() => {
                            dispatch(cleanAlarm({filter: props.filter}))
                        }}>
                    Závadu jsem odstranil/a
                </Button>
            </div>}
        </Paper>
    </Grid>
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