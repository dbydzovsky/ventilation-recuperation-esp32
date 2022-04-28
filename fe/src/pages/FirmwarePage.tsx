import React from 'react'
import FiberManualRecordIcon from '@material-ui/icons/FiberManualRecord';
import LooksOneIcon from '@material-ui/icons/LooksOne';
import Looks3Icon from '@material-ui/icons/Looks3';
import LooksTwo from '@material-ui/icons/LooksTwo';
import TimerIcon from '@material-ui/icons/Timer';
import Grid from "@material-ui/core/Grid";
import {makeStyles} from "@material-ui/styles";
import {Theme} from "@material-ui/core/styles";
import GitHubIcon from "@material-ui/icons/GitHub";
import SystemUpdateAltIcon from '@material-ui/icons/SystemUpdateAlt';
import {Button, Container} from "@material-ui/core";
import {DebugMessagesPage} from "./DebugMessagesPage";
import {useSelector} from "react-redux";
import {RootState} from "../reducers";
import GetAppIcon from '@material-ui/icons/GetApp';


export function FirmwarePage() {
    const classes = useStyles();
    const appVersion = useSelector((state: RootState) => state.debugState.appVersion);
    return <>
        <Container>
            <div className={classes.root}>
                <h1>Firmware {appVersion}</h1>
                <Button startIcon={<SystemUpdateAltIcon/>} variant="text" onClick={()=> {
                    window.open("/update", "_blank")
                }}>Aktualizovat</Button>
            </div>
            <h2>Ladící zprávy</h2>

            <p><Button variant={"contained"}  color={"secondary"} startIcon={<GetAppIcon/>} onClick={() => {
                window.open("/a/debugd/", "_blank")}
            }>Stáhnout</Button>Ladící zprávy jsou uchovány pouze v paměti a budou ztraceny při vypnutí zařízení.</p>
        </Container>
        <DebugMessagesPage/>
        <Container>
            <h2>Changelog</h2>
            <p>2.0.0 - Úvodní verze</p>
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