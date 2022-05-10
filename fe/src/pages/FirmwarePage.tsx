import React from 'react'
import {makeStyles} from "@material-ui/styles";
import {Theme} from "@material-ui/core/styles";
import SystemUpdateAltIcon from '@material-ui/icons/SystemUpdateAlt';
import {Button, Container, InputLabel} from "@material-ui/core";
import {DebugMessagesPage} from "./DebugMessagesPage";
import {useDispatch, useSelector} from "react-redux";
import {RootState} from "../reducers";
import GetAppIcon from '@material-ui/icons/GetApp';
import Switch from "@material-ui/core/Switch";
import {setTrace} from "../actions/debug";


export function FirmwarePage() {
    const classes = useStyles();
    const dispatch = useDispatch();

    const debugState = useSelector((state: RootState) => state.debugState);
    const appVersion = debugState.appVersion;
    const traceEnabled = debugState.trace;

    return <>
        <Container>
            <div className={classes.root}>
                <h1>Firmware {appVersion}</h1>
                <Button startIcon={<SystemUpdateAltIcon/>} variant="text" onClick={()=> {
                    window.open("/update", "_blank")
                }}>Aktualizovat</Button>
            </div>

            <h2>Ladící zprávy</h2>
            <div >
                <div>
                    <InputLabel htmlFor={"traceId"}>
                        <b>Detailní logování</b> (efekt je aplikován až při aktivaci)
                    </InputLabel>
                    <Switch
                        checked={traceEnabled}
                        onChange={(e, newValue) => {
                            dispatch(setTrace({trace: newValue}))
                        }}
                        id={"traceId"}
                        color="secondary"
                        name={"Trace"}
                        inputProps={{'aria-label': 'secondary checkbox'}}
                    ></Switch>
                </div>
                <p><Button variant={"contained"}  color={"secondary"} startIcon={<GetAppIcon/>} onClick={() => {
                    window.open("/a/debugd/", "_blank")}
                }>Stáhnout</Button> <p>Ladící zprávy jsou uchovány pouze v paměti a budou ztraceny při vypnutí zařízení.</p></p>
            </div>
        </Container>
        <DebugMessagesPage/>
        <Container>
            <h2>Changelog</h2>
            <p><b>2.03</b> Úvodní verze</p>
        </Container>
    </>
}


const useStyles = makeStyles((theme: Theme) => ({
    root: {
        margin: "auto"
    },
    row: {
        marginTop: 20
    },
    group: {
        marginBottom: 20
    },
    flex: {
        display: "flex",
        alignItems: "center",
        justifyContent: "left",
    },
    header: {
        marginTop: 20,
        fontSize: 15
    },
    center: {
        textAlign: "center"
    }
}));