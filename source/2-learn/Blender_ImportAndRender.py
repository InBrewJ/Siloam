#
# Blender_ImportAndRender.py
# 
# Currently steps 1-7 of the Blender workflow - import a spheremap, rotate it 
# and add lights, then render RGB-D images
#
# This script can also import the DAE models downloaded from the trimble warehouse
# and render them using two separate camera rigs - either a circular rig or a 
# square/rectangular rig. Or both if it takes your fancy
#
# Step 8 of the Blender workflow is to try to clean up the meshes with meshlab
# and see if SVM performance improves. This is probabaly for future work.
#
# It's advisable to save a startup file with a blank scene
# (i.e. with the cube and lamp deleted) before running this script
#
# There's a fair amount of repeated code - yes it needs refactoring, granted.
# The camera naming is a bit all over the placing when comparing SS and SL
# too. The paths are also all over the place and are very original-dev-specific
#

import bpy                    # for all blender functionality
import os                     # for os file handling
import math                   # literally just for pi
from shutil import copyfile   
import numpy                  # to create ranges
import mathutils              # for sin and cos 

# Set up some globals

fov = 90.0
objName = "map.obj" # the name SiloamSee gives to every exported obj
sceneName = "Scene"
daeCamGroup = []
daeRenderPath = []
daeCameraDimensions = []

def clearScene():
    # Just to be pythonic and explicit
    global daeCamGroup
    global daeRenderPath
    
    for object in bpy.data.objects:
        # print(object.name + " is at location: " + str(object.location) + " at present")
        bpy.data.objects[object.name].select = True
        bpy.ops.object.delete()
    
    daeCamGroup = []
    daeRenderPath = []
        
def toRad(angle):
    return angle*(math.pi/180.0)

def toDeg(angle):
    return 180*(angle/math.pi)

def createSSCameras(scene, context):
    ffcameraId = 0
    dfcameraId = 0
    for angle in [0, 45, 90, 135, 180, 225, 270, 315]:
        
        # Create the forwards facing cameras (two pillars)
        
        for z in [-1.0, -0.6]:
            # name the camera appropriately
            
            cameraData = bpy.data.cameras.new("forwards" + str(ffcameraId))
            camera = bpy.data.objects.new("forwards" + str(ffcameraId), cameraData)
            camera.location = (-0.2, 0.0, z)
            # Euler angles are in radians NOT degrees
            camera.rotation_euler = (toRad(90), 0.0, toRad(-90 + angle))
            camera.data.angle = toRad(fov)
        
            scene.objects.link(camera)
            
            ffcameraId += 1
        
        # create the downwards facing cameras to view the heading widget

        # name the camera appropriately
        
        cameraData = bpy.data.cameras.new("downwards"  + str(dfcameraId))
        camera = bpy.data.objects.new("downwards"  + str(dfcameraId), cameraData)
        camera.location = (-0.2, 0.0, -1.0)
        # Euler angles are in radians NOT degrees
        camera.rotation_euler = (0.0, 0.0, toRad(-90 + angle))
        camera.data.angle = toRad(fov)
    
        scene.objects.link(camera)
        
        dfcameraId += 1
        
# For the V2 dae camera rig : near = daeCameraDimensions[0] (x) (length)
#                           : far  = daeCameraDimensions[1] (y) (width)
        
def renderDepthMap(c, mode, folder, near, far):
    global daeCameraDimensions
    
    # switch on nodes and get reference
    bpy.context.scene.use_nodes = True
    tree = bpy.context.scene.node_tree
    
    #print("Removing default nodes")
    
    # clear default nodes
    for node in tree.nodes:
        tree.nodes.remove(node)
        
    # Create the render layers, map range and composite nodes
        
    RLNode = tree.nodes.new(type='CompositorNodeRLayers')
    RLNode.location = 0,0
    
    MRNode = tree.nodes.new(type='CompositorNodeMapRange')
    MRNode.location = 210,-100
    
    dimX = near
    dimY = far
    currentX = c.location[0]
    currentY = c.location[1]
    
    # set the distance thresholds
    if mode == "SS":
        MRNode.inputs[1].default_value = near
        MRNode.inputs[2].default_value = far
    elif mode == "SL":
        
        currentX = c.location[0]
        currentY = c.location[1]
        
        #dimX = daeCameraDimensions[0]
        #dimY = daeCameraDimensions[1]
        # "near" is the length of the box and "far" is the width...
        dimX = near
        dimY = far
        
        # This is a very explicit way to do it - there is of course
        # a more compact way, like the one found in createDAECamerasV2
        # but this allows greater tuning of the near/far variables
        
        # for near 0.3 is an experimental near factor
        
        # Corners first
        if (currentX == 0 and currentY == 0) or \
           (currentX == 0 and currentY == dimY-1) or \
           (currentX == dimX-1 and currentY == 0) or \
           (currentX == dimX-1 and currentY == dimY-1):
            MRNode.inputs[1].default_value = 0.3
            # + 0.5 is an experimental factor
            MRNode.inputs[2].default_value = math.sqrt((dimX**2.0) + (dimY**2.0)) + 0.5
            
        elif (currentY == 0 and (currentX > 0 and currentX < dimX-1)) or \
             (currentY == dimY-1 and (currentX > 0 and currentX < dimX-1)):
            # Bottom and top edges
            MRNode.inputs[1].default_value = 0.3
            MRNode.inputs[2].default_value = float(dimY)/2.0 + 1.0
            
            # check the z euler angle for the rotation of the camera
            # about the z axis
            
            if int(toDeg(c.rotation_euler[2])) % 90 != 0:
                MRNode.inputs[2].default_value = math.sqrt((dimX**2.0) + (dimY**2.0)) + 0.5
            else:
                MRNode.inputs[2].default_value = float(dimY)/2.0 + 1.0
            
        elif ((currentX == 0 or currentX == dimX-1) and (currentY > 0 and currentY < dimY)):
            # Side edges   
            MRNode.inputs[1].default_value = 0.3
            MRNode.inputs[2].default_value = float(dimX)
    else:
        print("Mode must be [SS or SL], try again!")
            
    # debugging
    if c.name == "rig156":
        print("dimX: " + str(dimX))
        print("dimY: " + str(dimY))
        print("currentX: " + str(currentX))
        print("currentY: " + str(currentY))
        print("Location of 156: " + str(c.location))
        print("Rotation of 156: " + str(c.rotation_euler))
        print("Near for " + str(c.name) + ": " + str(MRNode.inputs[1].default_value))
        print("Far for " + str(c.name) + ": " + str(MRNode.inputs[2].default_value))
            
    # Black is far, white is near  
    MRNode.inputs[3].default_value = 1.000
    MRNode.inputs[4].default_value = 0.000
    
    CNode = tree.nodes.new(type='CompositorNodeComposite')
    CNode.location = 400,0
    
    # link the nodes
    
    links = tree.links
    link = links.new(RLNode.outputs[2], MRNode.inputs[0])
    link = links.new(MRNode.outputs[0], CNode.inputs[0])
    
    # render the depth image
    if mode == "SS":
        # For SiloamSee renders
        bpy.data.scenes[objName].render.filepath = os.path.join(*["/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/renders/SiloamSee", folder, c.name + "-D"])
    elif mode == "SL":
        # For SiloamLearn (trimble model) renders
        bpy.data.scenes[sceneName].render.filepath = os.path.join(*["/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/renders/SiloamLearn", folder, c.name + "-D"])
        
    bpy.ops.render.render(write_still=True)
        
    # clear nodes again and disable nodes
    for node in tree.nodes:
        tree.nodes.remove(node)
     
    bpy.context.scene.use_nodes = False    
    
def renderSS(objPath, folder):
    
    # just to be safe (and for dev purposes)
    clearScene()
        
    # Set the context and the standard model path
    
    context = bpy.context
    
    modelPath = objPath + folder
    
    # Create a scene
    scene = bpy.data.scenes.new("Scene")
    
    # Create the cameras
    
    createSSCameras(scene, context)
        
    # Create an omidirectional point light without specular light to 
    # illuminate the textures as they were captured   
    
    # Point light creation
    
    lampData = bpy.data.lamps.new(name="Point Light", type='POINT')
    lampData.use_specular = False
    lampData.use_diffuse = True
    lampData.energy = 1.5
    lamp = bpy.data.objects.new(name="Point Light", object_data=lampData)
    lamp.location = (0.0, 0.0, 0.0)
    lamp.select = True
    scene.objects.active = lamp
    
    scene.objects.link(lamp)
    
    scene.update()
    
    # the following actually creates another scene and links the imported
    # object to it
    
    #scene.camera = camera
    path = os.path.join(modelPath, "map.obj")
    # make a new scene with cam and lights linked
    context.screen.scene = scene
    bpy.ops.scene.new(type='LINK_OBJECTS')
    context.scene.name = "map.obj"
    cams = [c for c in context.scene.objects if c.type == 'CAMERA']
    # import model
    bpy.ops.import_scene.obj(filepath=path, axis_forward='-Z', axis_up='Y', filter_glob="*.obj;*.mtl")
    
    bpy.data.scenes[objName].render.resolution_x = 400
    bpy.data.scenes[objName].render.resolution_y = 400
    bpy.data.scenes[objName].render.resolution_percentage = 100
    
    for c in cams:
        context.scene.camera = c                                    
        print("Render ", objName, context.scene.name, c.name)
        bpy.data.scenes[objName].render.filepath = os.path.join(*["/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/renders/SiloamSee", folder, c.name + "-RGB"])
        bpy.ops.render.render(write_still=True)
        
        # set up nodes for creating depth maps
        
        if "forwards" in c.name:
            print("Generating depth map for: " + c.name)
            renderDepthMap(c, "SS", folder, 0.720, 2.750)
             
    # copy the .dat file from the obj folder into the render folder
    print("Copying dat file...")
    copyfile(os.path.join(modelPath, "gpsAndHeading.dat"), os.path.join(*["/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/renders/SiloamSee", folder, "gpsAndHeading.dat"]))
        
def renderOnce():
    clearScene()
    renderSS("/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/models/obj/", "SiloamSee-170725-203831-train")

# runSS(objPath) function - to be called from the python console window

# My path for the OBJs from SiloamSee:
# "/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/models/obj/"

def runSS(objPath):
    
    # find all the sub folders within objPath and then render all the images
    subdirs = [x[0] for x in os.walk(objPath)]
    
    # the first in the list is the directory itself, so we take all
    # apart from the first, split by "/" and just take the end of
    # this list and pass it to our render function
    
    folders = []
    
    for subdir in subdirs[1:]:
        folders.append(subdir.split("/")[-1])
    
    print("Rendering...")
    
    for folder in folders:
        renderSS(objPath, folder)
    
    print("Done")
    
def createDAECamerasV1(radius, scene, context, daeModel):
    
    # We create a cube at the origin for the cameras to focus on
    # Each of the DAE's is then moved by hand to where the cameras
    # are focussing. It's possible with some of the DAE's to set 
    # to origin, but with others it isn't. Workflow error

    # Add a Cube primitive to the empty object.
    bpy.ops.mesh.primitive_cube_add(location=(0.0, 0.0, 1.5))
    bpy.data.objects["Cube"].hide = True
    bpy.data.objects["Cube"].hide_render = True
    bpy.data.objects["Cube"].name = "CameraAnchor"
    
    cameraId = 0
    # 5 heights: [0.5, 1.0, 1.5, 2.0, 2.5]
    
    for z in [0.5, 1.0, 1.5, 2.0, 2.5]:
        for angle in numpy.arange(0, 360, 15):
            
            # set up the lights at the same time, at 12, 3, 6 and 9
            
            # Point light creation
    
            if z == 1.5 and angle in [0, 90, 180, 270]:
                lampData = bpy.data.lamps.new(name="Point Light", type='POINT')
                lampData.use_specular = False
                lampData.use_diffuse = True
                lampData.energy = 1.0
                lamp = bpy.data.objects.new(name="Point Light", object_data=lampData)
                lamp.location = (radius*math.cos(toRad(angle)), radius*math.sin(toRad(angle)), z)
                lamp.select = True
                scene.objects.active = lamp
                
                scene.objects.link(lamp) 
             
            # name the camera appropriately
            cameraData = bpy.data.cameras.new("rig" + str(cameraId))
            camera = bpy.data.objects.new("rig" + str(cameraId), cameraData)
            camera.data.angle = toRad(fov)
            
            # location cameras on the circumference of a circle
            # at 15 degree intervals using parametric circular equations
            
            camera.location = (radius*math.cos(toRad(angle)), radius*math.sin(toRad(angle)), z) 
           
            # Make each camera look at the centroid of the "Sketchup" object
            
            daeCamGroup.append(bpy.data.objects["rig" + str(cameraId)])
           
            o = bpy.data.objects["rig" + str(cameraId)]
            constraint = o.constraints.new('TRACK_TO')
            constraint.target = bpy.data.objects["CameraAnchor"]
            constraint.track_axis = 'TRACK_NEGATIVE_Z'
            constraint.up_axis = 'UP_Y'
        
            scene.objects.link(camera)
            
            cameraId += 1
            
    scene.update()
    
    # make sure we deselect all the lamps
    
    for obj in bpy.data.objects:
        obj.select = False
        
# createDAECamerasV2(x, y, scene, context, daeModel)
#
# Creates pillars of cameras in a rectangle (or, of course, square)
# around a given object. Grab em and delete em if the spacing looks wrong,
# but the cameras should be fairly close to the subject from trimble
#
# For lack of a better method it also creates a point light at the centre
# of every camera rig. It's probably overkill but at least it's even
#
# Also be sure to clean up the trimble model before rendering!
        
def createDAECamerasV2(x, y, scene, context, daeModel):    
    coords = []
    global daeCameraDimensions
    
    daeCameraDimensions = [x, y]
    
    print(daeCameraDimensions)
    
    # creates the coordinates where each camera pillar will be drawn
    for loopY in range(y):
        for loopX in range(x):
              
            # Add the top side
            if loopY == y-1:
                coords.append((loopX,loopY))
                
            # Add the bottom side
            if loopY == 0:
                coords.append((loopX,loopY))
                
             # Add middles
            if loopY > 0 and loopY < y-1:
                if loopX == 0 or loopX == x-1:
                    coords.append((loopX,loopY))   
    
    cameraId = 0
    for camX,camY in coords:
        for camZ in [0.0, 0.4, 0.8, 1.2]: 
            for angle in [0, 45, 90, 135, 180, 225, 270, 315]:
                
                # name the camera appropriately
                
                cameraData = bpy.data.cameras.new("rig" + str(cameraId))
                camera = bpy.data.objects.new("rig" + str(cameraId), cameraData)
                camera.location = (camX, camY, camZ)
                # Euler angles are in radians NOT degrees
                camera.rotation_euler = (toRad(90), 0.0, toRad(-90 + angle))
                camera.data.angle = toRad(fov)
            
                scene.objects.link(camera)
                
                daeCamGroup.append(bpy.data.objects["rig" + str(cameraId)])
                
                cameraId += 1
                
                if camZ == 0.8 and angle == 90:
                    lampData = bpy.data.lamps.new(name="Point Light", type='POINT')
                    lampData.use_specular = False
                    lampData.use_diffuse = True
                    lampData.energy = 0.2
                    lamp = bpy.data.objects.new(name="Point Light", object_data=lampData)
                    lamp.location = (camX, camY, camZ)
                    lamp.select = True
                    scene.objects.active = lamp
                    scene.objects.link(lamp)
            
    scene.update()
    
    # make sure we deselect all the lamps (and everything else for that matter)
    
    for obj in bpy.data.objects:
        obj.select = False 
                
#
# setupDAEEnv(version, radius, x, y) - to be called from the Python console window
# The rendering of Trimble models is a slightly more interactive process
# since the points of origin on the DAE's vary slightly. setupEnvTrimble sets up
# an array of cameras and lights around the object. It should be followed by
# importDAE(daePath) and renderDAE() which will output the RGB-D data to a 
# predefined folder
#
    
def setupDAEEnv(version, radius, x, y):
    
    global daeCameraDimensions
    
    print("Setting up DAE cameras...")
        
    # Set the context and the standard model path
    
    context = bpy.context
    
    if bpy.data.objects.get('SketchUp') is None:
        print("Please import a dae from SketchUp first...")
        return
    else:
        if version is "V1":
            createDAECamerasV1(radius, bpy.context.screen.scene, context, bpy.data.objects['SketchUp'])
        elif version is "V2":
            createDAECamerasV2(x, y, bpy.context.screen.scene, context, bpy.data.objects['SketchUp'])
            
        else:
            print("Version must be [V1 or V2]")

    print("Done")
    grabSketchUp()
    print("SketchUp model is grabbed. Move at will...")
    
# gradCams() - simple function that selects all the cameras so they can be
# translated as a group
    
def grabCams():
    for cam in daeCamGroup:
        cam.select = False
        cam.select = True
        
# IMPORTANT
# MOVE THE DAE INTO THE CAMERAS NOT THE CAMERAS AROUND THE DAE
# Doing this incorrectly will mess up the depth map rendering 
        
def grabSketchUp():
    scene = bpy.context.scene
    for ob in scene.objects:
        if ob.name.startswith("SketchUp"):
            ob.select = False
            ob.select = True
        
def renderSL():
    global daeRenderPath
    global daeCameraDimensions
    
    if daeRenderPath == []:
        print("Please import your DAE via ImportAndRender.importDAE(<daePath>). If you already have a DAE imported, delete it and start again")
        return  
    elif bpy.data.objects.get('SketchUp') is None or bpy.data.objects.get('rig0') is None:
        print("Please import a dae from SketchUp and then run setupDAEEnv() - see script for details")
        return
    
    print("Rendering RGB-D from current DAE. This might take a while...")
    
    # Scene name should always just be "Scene" - we're not deviating from default
    sceneName = "Scene"
    context = bpy.context
    cams = [c for c in context.scene.objects if c.type == 'CAMERA']
    
    for c in cams:
        context.scene.camera = c
        bpy.data.scenes[sceneName].render.resolution_x = 400
        bpy.data.scenes[sceneName].render.resolution_y = 400
        bpy.data.scenes[sceneName].render.resolution_percentage = 100
        
        # render RGB    
        context.scene.render.filepath = os.path.join(daeRenderPath, c.name + "-RGB")
        bpy.ops.render.render(write_still=True)
    
        # render D
        #print("Generating depth map for: " + c.name)
        renderDepthMap(c, "SL", daeRenderPath.split("/")[-1], daeCameraDimensions[0], daeCameraDimensions[1])
        
    print("Done")
        
    
# My path for the DAE's from Trimble Warehouse:
# "/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/models/DAE"
# Test on the fancy door:
# "/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/models/DAE/Doors/fancyDoor/model.dae"
# Test on archdoor:
# "/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/models/DAE/Doors/ArchDoor.dae"
# Don't import via the standard Blender GUI method - it does not set things up correctly for 
# the output render path for the current DAE, as can be seen below

# Standard render path is: 
# /Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/renders/SiloamLearn/
# plus the name of the DAE file. If the DAE file is just called model.dae, make it the name of the
# parent directory - e.g. ".../fancyDoor/model.dae" would become "fancyDoor"
# we change the global "daeRenderPath" here

def importDAE(daePath):
    global daeRenderPath
    
    tempPath = daePath
    basePath = []
    
    tempPath = tempPath.split("/")
    
    if tempPath[-1] == "model.dae":
        # go one up in the directory hierarchy
        basePath = tempPath[-2]
    else: 
        # use the basePath of tempPath (which is daePath)
        basePath = tempPath[-1].split(".")[0]
    
    daeRenderPath = os.path.join("/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/renders/SiloamLearn/", basePath)
    
    bpy.ops.wm.collada_import(filepath=daePath)
    
def testRenderSL():
    importDAE("/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/models/DAE/Doors/ArchDoor.dae")
    setupDAEEnv("V2", 0, 7, 4)
    grabSketchUp()
    print("After tweaking the position, now run \"ImportAndRender.renderSL()\"")
