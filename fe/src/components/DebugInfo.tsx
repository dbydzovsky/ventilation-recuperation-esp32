import React from 'react';
import MemoryIcon from '@material-ui/icons/Memory';
import {makeStyles} from "@material-ui/styles";
import Grid from "@material-ui/core/Grid";
import {useSelector} from "react-redux";
import {RootState} from "../reducers";
import OfflineBoltIcon from '@material-ui/icons/OfflineBolt';

export function DebugInfo(){
    const state = useSelector((state: RootState) => state.state);
    const classes = useStyles();
    return <div className={classes.root}>
             <Grid container>
                 <Grid item xs={12}>
                     <MemoryIcon/> Volná paměť (bytes): {state.heap}
                 </Grid>
                 <Grid item xs={12}>
                     <OfflineBoltIcon/> Počet restartů: {state.restarts}
                 </Grid>
             </Grid>
    </div>
}

const useStyles = makeStyles({
    root: {
        textAlign: "left",
        margin: 10
    },
});