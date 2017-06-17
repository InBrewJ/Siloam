
<map version="0.9.0">
    <node TEXT="Geospatial Macro Level Feature Database (Outdoor Feature Mapping)" FOLDED="false">
        <edge COLOR="#b4b4b4" />
        <font NAME="Helvetica" SIZE="10" />
        <node TEXT="New Obstacles" FOLDED="false" POSITION="left">
            <edge COLOR="#efa670" />
            <font NAME="Helvetica" SIZE="10" />
            <node TEXT="Another machine learning method without depth information (i.e. 2D feature detection) - might include some sign recognition? ANN OCR?" FOLDED="false">
                <edge COLOR="#efa776" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
            <node TEXT="OpenSURF" FOLDED="false">
                <edge COLOR="#efa26c" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
            <node TEXT="Like &quot;Beme&quot; - hold any reasonable smartphone to the user's chest and infer an obstacle" FOLDED="false">
                <edge COLOR="#eea16b" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
            <node TEXT="Then re-route the user accordingly" FOLDED="false">
                <edge COLOR="#eea979" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
        </node>
        <node TEXT="RGB-D Machine Learning" FOLDED="false" POSITION="right">
            <edge COLOR="#67d7c4" />
            <font NAME="Helvetica" SIZE="10" />
            <node TEXT="See &quot;Sliding Shapes for 3D Object Detection in Depth Images&quot; (do a google scholar search)" FOLDED="false">
                <edge COLOR="#5cd6bf" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
            <node TEXT="Needs to learn where doors are" FOLDED="false">
                <edge COLOR="#64d6c1" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
        </node>
        <node TEXT="Tango Data Collection" FOLDED="false" POSITION="right">
            <edge COLOR="#7aa3e5" />
            <font NAME="Helvetica" SIZE="10" />
            <node TEXT="Needs applet developing to take photospheres with depth information" FOLDED="false">
                <edge COLOR="#84ace7" />
                <font NAME="Helvetica" SIZE="10" />
                <node TEXT="The photosphere feature should be open source - have a look at: https://android.googlesource.com/platform/packages/apps/Camera/+refs" FOLDED="false">
                    <edge COLOR="#86afe6" />
                    <font NAME="Helvetica" SIZE="10" />
                </node>
                <node TEXT="Version of Android (that can probably be updated) on the P2P is 6.0.1 - Marshmallow" FOLDED="false">
                    <edge COLOR="#8eb4e9" />
                    <font NAME="Helvetica" SIZE="10" />
                </node>
            </node>
            <node TEXT="Needs kerb heights measured" FOLDED="false">
                <edge COLOR="#81aae8" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
            <node TEXT="Steps too - i.e. by Augustus John pub" FOLDED="false">
                <edge COLOR="#83abe8" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
            <node TEXT="Texture information is also important? Pretty sure Neil mentioned this" FOLDED="false">
                <edge COLOR="#7aa6e7" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
            <node TEXT="Possible extensibility for public transport information (OCR? Link with Google maps?)
" FOLDED="false">
                <edge COLOR="#709fe5" />
                <font NAME="Helvetica" SIZE="10" />
                <node TEXT="OSM does not have public transport routes" FOLDED="false">
                    <edge COLOR="#689ce5" />
                    <font NAME="Helvetica" SIZE="10" />
                </node>
                <node TEXT="But there might be a Google maps API?" FOLDED="false">
                    <edge COLOR="#76a4e5" />
                    <font NAME="Helvetica" SIZE="10" />
                </node>
            </node>
        </node>
        <node TEXT="Data Organisation and Fetching" FOLDED="false" POSITION="left">
            <edge COLOR="#9ed56b" />
            <font NAME="Helvetica" SIZE="10" />
            <node TEXT="Upload 360 photospheres to Street View" FOLDED="false">
                <edge COLOR="#9bd264" />
                <font NAME="Helvetica" SIZE="10" />
                <node TEXT="Uploads take ~1 day (further testing will commence)" FOLDED="false">
                    <edge COLOR="#9cd064" />
                    <font NAME="Helvetica" SIZE="10" />
                </node>
            </node>
            <node TEXT="2D images downloaded back from StreetView [Point of re-download?](#035691)" FOLDED="false">
                <edge COLOR="#a4d870" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
        </node>
        <node TEXT="Route Mapping (OSM Routes with macro level features overlayed)" FOLDED="false" POSITION="right">
            <edge COLOR="#ebd95f" />
            <font NAME="Helvetica" SIZE="10" />
            <node TEXT="Get routes via OSM via web API" FOLDED="false">
                <edge COLOR="#edd967" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
            <node TEXT="Add in features from server database" FOLDED="false">
                <edge COLOR="#ebd761" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
            <node TEXT="Present route with features" FOLDED="false">
                <edge COLOR="#edd961" />
                <font NAME="Helvetica" SIZE="10" />
                <node TEXT="Might need further (supervised) machine learning to determine USEFUL features - i.e. waypoints useful for VI users" FOLDED="false">
                    <edge COLOR="#eed864" />
                    <font NAME="Helvetica" SIZE="10" />
                </node>
                <node TEXT="Add in weather information? (Probably for future work)" FOLDED="false">
                    <edge COLOR="#ecd76a" />
                    <font NAME="Helvetica" SIZE="10" />
                </node>
            </node>
            <node TEXT="Headings of 2D images compared with headings of OSM routes - maybe?" FOLDED="false">
                <edge COLOR="#ecd862" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
        </node>
    </node>
</map>