const gulp = require('gulp')
const inlinesource = require('gulp-inline-source')
const replace = require('gulp-replace')
var concat = require('gulp-concat');
var inlineImages = require('gulp-inline-images');

gulp.task('default', () => {
    return gulp.src('./build/*.html')
        // .pipe(replace('.js"></script>', '.js" inline></script>'))
        .pipe(replace('rel="stylesheet">', 'rel="stylesheet" inline>'))
        .pipe(inlineImages({/* options */}))
        .pipe(inlinesource({
            compress: true,
            ignore: ['png']
        }))
        .pipe(gulp.dest('./build'))
});
