## Package startup
# Calling Octave functions when it's not embedded causes segfault, and calling
# functions from M files that aren't on Octave's load path causes an error.
# This function will set everything up nicely, or complain if it can't:
setup_Octave <- function() {
    packageStartupMessage("Embedding Octave...")
    embed_octave_output <- capture.output(.embed_octave(TRUE, FALSE))
    if ( rev(embed_octave_output)[1] == "[1] TRUE" ) {
        packageStartupMessage("Octave successfully embedded.")
        sub_dirs <- c("cov", "doc", "inf", "lik", "mean", "prior", "util",
                      "util/lbfgsb")
        dirs_to_add <- system.file(paste0("gpml", c("", paste0("/", sub_dirs))),
                                   package = "gpmlr")
        path_warn <- capture.output(.add_to_path(dirs_to_add))#, type = "message")
        # .add_to_path(dirs_to_add)
        packageStartupMessage("Octave load path correctly set.")
        packageStartupMessage("gp() is safe to call.")
    } else {
        issues_url <- "https://github.com/duckmayr/gpmlr/issues"
        stop("Octave failed to embed; calling gp() will crash R.\n",
             "Please report this error at ", issues_url, ".\n",
             "Be sure to mention your operating system, ",
             "and your R and Octave versions.")
    }
}
# Ideally Octave will be embedded the whole time our package is attached,
# with the Octave load path set appropriately. We do this in .onAttach().
.onAttach <- function(libname, pkgname) {
    setup_Octave()
}

## Package cleanup
# We terminate Octave before we quit R, or it causes a segfault.
# We make sure this occurs by creating a dummy environment to use with
# reg.finalizer(), along with a function that exits Octave:
.gpmlr_package_loaded <- environment()
.exit_octave_on_quit <- function(e) try(.exit_octave(FALSE), silent = TRUE)
# We then call reg.finalizer() in .onLoad()
.onLoad <- function(libname, pkgname) {
    reg.finalizer(.gpmlr_package_loaded, .exit_octave_on_quit, onexit = TRUE)
}
# This is the typical .onUnload to unload our shared object.
.onUnload <- function(libpath) {
    library.dynam.unload('gpmlr', libpath)
}

