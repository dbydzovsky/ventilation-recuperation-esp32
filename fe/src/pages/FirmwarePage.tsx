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



export function FirmwarePage() {
    const classes = useStyles();
    return <Container>
        <div className={classes.root}>
            <h1>Firmware</h1>
            <p>verze 2.0</p>
            <p>@2022</p>
            <Button startIcon={<SystemUpdateAltIcon/>} variant="text" onClick={()=> {
                window.open("/update", "_blank")
            }}>Aktualizovat</Button>
        </div>
    </Container>
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