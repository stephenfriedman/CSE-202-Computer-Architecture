// This is the path to the in-memory cache... we don't want any configuration
// information in the main code folders, but we *do* want it in the
// repository for fast deployment.
module.exports = {
    'url' : '' // TODO: put the path to your memcached instance, such as 10.2.5.6:11211
}
