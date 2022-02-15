import React, {Dispatch, SetStateAction, useEffect} from 'react'
import {makeStyles, Theme} from "@material-ui/core/styles";
import {useActions} from "../actions";
import {useDispatch, useSelector} from "react-redux";
import {RootState} from "../reducers";
import {getConfiguration} from "../actions/configuration";
import {getSettings, saveSettings} from "../actions/settings";
import {Configuration} from "../model/configuration";
import {Settings} from "../model/settings";
import Grid from "@material-ui/core/Grid";
import {Button, Container, Input, InputLabel, Paper} from "@material-ui/core";
import BrandingWatermarkIcon from "@material-ui/icons/BrandingWatermark";

export interface Props {

}

export function SettingsPage(props: Props) {
    const classes = useStyles()
    const state = useSelector((state: RootState) => state.settings);
    const dispatch = useDispatch();
    useEffect(() => {
        dispatch(getSettings);
    }, []);
    const [values, setValues] = React.useState({...state.settings} as Settings);
    useEffect(() => {
        setValues(state.settings)
    }, [state])

    if (!values) {
        return <div>Loading..</div>
    }
    return <div className={classes.paper}>
            <h1>Pokročilé nastavení</h1>
            <Grid container>
                <Property values={values} setValues={setValues} type="boolean" property={"checkRecuperationRpm"}/>
                <Property values={values} setValues={setValues} type="boolean" property={"checkVentilatorRpm"}/>
                <Property values={values} setValues={setValues} type="number" property={"unblockingFansPeriod"}/>
                <Property values={values} setValues={setValues} type="number" property={"ventilatorMaxRpm"}/>
                <Property values={values} setValues={setValues} type="number" property={"recuperationMaxRpm"}/>
                <Property values={values} setValues={setValues} type="number" property={"syncForecastTolerateLastSuccessFor"}/>
                <Property values={values} setValues={setValues} type="number" property={"syncForecastInterval"}/>
                <Property values={values} setValues={setValues} type="number" property={"relayCooldown"}/>
                <Property values={values} setValues={setValues} type="number" property={"recuperationWaitForDirectionChange"}/>
                <Property values={values} setValues={setValues} type="number" property={"recuperationCycleDuration"}/>
                <Grid item xs={12} >
                    <Button variant={"contained"} color={"secondary"} onClick={() => {
                        dispatch(saveSettings(values))
                    }}>
                        Uložit nové nastavení
                    </Button>
                </Grid>
            </Grid>
        </div>
}

export interface PropertyProps {
    values: Settings
    setValues : Dispatch<SetStateAction<Settings>>
    property: string
    type: "number" | "boolean"
}

export function Property(props: PropertyProps ) {
    const classes = useStyles()
    const id = "standard-adornment-"+props.property
    return <Grid item xs={12}>
            <Paper className={classes.paper}>
                <InputLabel htmlFor={id} className={classes.label}>
                {props.property} ({props.type})
            </InputLabel>
            <Input
                id={id}
                value={(props.values as any)[props.property] as string}
                onChange={(e) => {
                    let value : any = e.target.value
                    // if (props.type == "number") {
                    //     value = Number(value)
                    // } else if (props.type=="boolean") {
                    //     value = Boolean(value)
                    // }
                    props.setValues({...props.values, [props.property]: value})
                }}
                placeholder={props.property}
            />

            </Paper>
        </Grid>

}

const useStyles = makeStyles((theme: Theme) => {
    return {
        paper: {
            margin: 20,
            padding: 20
        },
        label: {

        }
    }
})