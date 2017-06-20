
<map version="0.9.0">
    <node TEXT="Geospatial Macro Level Feature Database (Outdoor Feature Mapping) with an open source feel aka &quot;Siloam&quot;" FOLDED="false">
        <edge COLOR="#b4b4b4" />
        <font NAME="Helvetica" SIZE="10" />
        <node TEXT="RGB-D Machine Learning" FOLDED="false" POSITION="right">
            <edge COLOR="#67d7c4" />
            <font NAME="Helvetica" SIZE="10" />
            <node TEXT="See &quot;Sliding Shapes for 3D Object Detection in Depth Images&quot; (do a google scholar search)" FOLDED="false">
                <edge COLOR="#5cd6bf" />
                <font NAME="Helvetica" SIZE="10" />
                <node TEXT="Probably will required a lot of storage. Is there any available for masters students?" FOLDED="false">
                    <edge COLOR="#54d4b9" />
                    <font NAME="Helvetica" SIZE="10" />
                </node>
            </node>
            <node TEXT="Needs to learn where entrances to buildings are" FOLDED="false">
                <edge COLOR="#64d6c1" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
            <node TEXT="How much space is there for deviation from the original design document?" FOLDED="false">
                <edge COLOR="#6ddac6" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
        </node>
        <node TEXT="New Obstacles" FOLDED="false" POSITION="left">
            <edge COLOR="#efa670" />
            <font NAME="Helvetica" SIZE="10" />
            <node TEXT="Don't necessarily need to fully classify the obstacle - might just be &quot;an obstacle&quot; for the scope of this particular project" FOLDED="false">
                <edge COLOR="#f0aa79" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
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
            <node TEXT="Might end up being in further research. But it would be a great feature to have" FOLDED="false">
                <edge COLOR="#ee9f69" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
        </node>
        <node TEXT="Tango Data Collection - half C++ half Java. I.e. Identifying objects  on route" FOLDED="false" POSITION="right">
            <edge COLOR="#7aa3e5" />
            <font NAME="Helvetica" SIZE="10" />
            <node TEXT="Investigation needed for effectiveness of outdoor Tango in the outdoors at daytime. Might be best to map in early evening/night. Street View does not have day/night cycles? See: https://stackoverflow.com/questions/32239932/does-the-project-tango-tablet-work-outdoors and video: https://www.youtube.com/watch?v=DhdaV3NmN88 and also video: https://www.youtube.com/watch?v=DhdaV3NmN88" FOLDED="false">
                <edge COLOR="#85ace6" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
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
            <node TEXT="NB: photospheres uploaded successfully to google do NOT have blurred out numberplates etc. The app does have the function to blur things out manually however. Would increase mapping time but would keep everything above board " FOLDED="false">
                <edge COLOR="#7ea7e5" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
        </node>
        <node TEXT="Data Organisation and web API Fetching" FOLDED="false" POSITION="left">
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
            <node TEXT="2D images downloaded back from StreetView [Point of re-download](#035691)" FOLDED="false">
                <edge COLOR="#a4d870" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
            <node TEXT="Models of features could be derived from manual photogrammetry. (i.e. features that will train the machine learning classifier of new features found in the local liverpool area)" FOLDED="false">
                <edge COLOR="#9cd167" />
                <font NAME="Helvetica" SIZE="10" />
                <node TEXT="Could be done via the exisiting streetview imagery as Neil mentioned" FOLDED="false">
                    <edge COLOR="#9ace62" />
                    <font NAME="Helvetica" SIZE="10" />
                </node>
                <node TEXT="Or photogrammetry from 360 images taken with the tango device" FOLDED="false">
                    <edge COLOR="#a1d36b" />
                    <font NAME="Helvetica" SIZE="10" />
                </node>
            </node>
            <node TEXT="Relative positions of features stored in something like a PostGIS database" FOLDED="false">
                <edge COLOR="#a3d86e" />
                <font NAME="Helvetica" SIZE="10" />
                <node TEXT="GeoCouch?" FOLDED="false">
                    <edge COLOR="#a6d96e" />
                    <font NAME="Helvetica" SIZE="10" />
                </node>
            </node>
        </node>
        <node TEXT="Route Mapping (OSM Routes with macro level features overlayed)" FOLDED="false" POSITION="right">
            <edge COLOR="#ebd95f" />
            <font NAME="Helvetica" SIZE="10" />
            <node TEXT="Predefined exploration paths" FOLDED="false">
                <edge COLOR="#e9d457" />
                <font NAME="Helvetica" SIZE="10" />
                <node TEXT="e.g." FOLDED="false">
                    <edge COLOR="#e9d257" />
                    <font NAME="Helvetica" SIZE="10" />
                    <node TEXT="Keep as predefined routes" FOLDED="false">
                        <edge COLOR="#e9cf55" />
                        <font NAME="Helvetica" SIZE="10" />
                    </node>
                    <node TEXT="Explore entire campus" FOLDED="false">
                        <edge COLOR="#ead155" />
                        <font NAME="Helvetica" SIZE="10" />
                        <node TEXT="Load entire area of geocoded features" FOLDED="false">
                            <edge COLOR="#ead161" />
                            <font NAME="Helvetica" SIZE="10" />
                        </node>
                        <node TEXT="Highlight key waypoints" FOLDED="false">
                            <edge COLOR="#ecd15b" />
                            <font NAME="Helvetica" SIZE="10" />
                        </node>
                    </node>
                    <node TEXT="Explore North Campus" FOLDED="false">
                        <edge COLOR="#e6cc52" />
                        <font NAME="Helvetica" SIZE="10" />
                    </node>
                    <node TEXT="Explore Engineering" FOLDED="false">
                        <edge COLOR="#e9d161" />
                        <font NAME="Helvetica" SIZE="10" />
                    </node>
                </node>
            </node>
            <node TEXT="Get routes via OSM via web API" FOLDED="false">
                <edge COLOR="#edd967" />
                <font NAME="Helvetica" SIZE="10" />
                <node TEXT="OSM does GPS traces" FOLDED="false">
                    <edge COLOR="#eed972" />
                    <font NAME="Helvetica" SIZE="10" />
                </node>
            </node>
            <node TEXT="Add in features from server database" FOLDED="false">
                <edge COLOR="#ebd761" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
            <node TEXT="Present route with features to user" FOLDED="false">
                <edge COLOR="#edd961" />
                <font NAME="Helvetica" SIZE="10" />
                <node TEXT="Routes via Mapzen or GraphHopper (both of which are proven to work with OSM)" FOLDED="false">
                    <edge COLOR="#ecd456" />
                    <font NAME="Helvetica" SIZE="10" />
                </node>
                <node TEXT="Might need further (supervised) machine learning to determine USEFUL features - i.e. waypoints useful for VI users" FOLDED="false">
                    <edge COLOR="#eed864" />
                    <font NAME="Helvetica" SIZE="10" />
                </node>
                <node TEXT="Add in weather information? (Probably for future work)" FOLDED="false">
                    <edge COLOR="#ecd76a" />
                    <font NAME="Helvetica" SIZE="10" />
                </node>
            </node>
            <node TEXT="Compass headings of 2D images compared with headings of OSM routes - to find correct features in the path of the user. But the features will already be in the database - might be more useful for the RGB-D learning step. But will introduce a lot of (possible quite useful) redundancy due to each feature being captured multiple times" FOLDED="false">
                <edge COLOR="#ecd862" />
                <font NAME="Helvetica" SIZE="10" />
            </node>
        </node>
    </node>
</map>