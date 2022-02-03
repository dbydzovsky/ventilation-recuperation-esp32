import React from 'react';
import {useSelector} from "react-redux";
import {RootState} from "../reducers";
import {makeStyles} from "@material-ui/core/styles";
import {Backdrop, CircularProgress} from "@material-ui/core";

export function Loader() {
    const loader = useSelector((state: RootState) => state.loader);
    const classes = useStyles();

    return <Backdrop className={classes.backdrop} open={loader.loading}>
        <CircularProgress color="inherit" />
    </Backdrop>
}

const useStyles = makeStyles((theme) => ({
    backdrop: {
        zIndex: theme.zIndex.drawer + 1,
        color: '#fff',
    },
}));
