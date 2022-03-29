import React from 'react'
import {FilterPage} from "./FilterPage";
import {AlarmPage} from "./AlarmPage";
import Grid from "@material-ui/core/Grid";

export function MaintenancePage() {
    return <Grid container>
        <FilterPage/>
        <AlarmPage/>
    </Grid>
}