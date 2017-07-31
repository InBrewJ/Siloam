#
# testImport.py
# 
# Steps 1 and 2 of the Blender workflow - import a spheremap, rotate it 
# correctly (or at least set up the axis correctly), add lights
# and switch to texture mode
#
# It's advisable to save a startup file with a blank scene
# (i.e. with the cube and lamp deleted) before running this script
#
#

import bpy   # for all blender functionality
import os    # for os file handling
import math

# Do a test import of the seventh(?) SiloamSee OBJ/MTL model

# Set up some camera variables

fov = 90.0

def clearScene():
    for object in bpy.data.objects:
        print(object.name + " is at location: " + str(object.location) + " at present")
        bpy.data.objects[object.name].select = True
        bpy.ops.object.delete()
        
def toRad(angle):
    return angle*(math.pi/180.0)

# run() function - to be called from the python console window

def run(folder):
    
    # Print the locations of the objects in the opening scene
    # (should just be the cube, the light and the camera),
    # select them and then delete them to start with a blank canvas
    
    # just to be safe (and for dev purposes)
    clearScene()
        
    # Import a sample object (the full-ish bus stop spheremap)
    
    context = bpy.context
    
    modelPath = "/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/models/obj/" + folder
    siloamSeeMaps = ["map.obj"]
    
    # Create a scene
    scene = bpy.data.scenes.new("Scene")
    
    # Create the cameras
    for angle in [0, 45, 90, 135, 180, 225, 270, 315]:
        
        # Create the forwards facing cameras
        
        cameraData = bpy.data.cameras.new("Camera")
        camera = bpy.data.objects.new("Camera", cameraData)
        camera.location = (-0.2, 0.0, -0.6)
        # Euler angles are in radians NOT degrees
        camera.rotation_euler = (toRad(90), 0.0, toRad(-90 + angle))
        camera.data.angle = toRad(fov)
    
        scene.objects.link(camera)
        
        # create the downwards facing cameras to view the heading widget
        
        cameraData = bpy.data.cameras.new("Camera")
        camera = bpy.data.objects.new("Camera", cameraData)
        camera.location = (-0.2, 0.0, -0.6)
        # Euler angles are in radians NOT degrees
        camera.rotation_euler = (0.0, 0.0, toRad(-90 + angle))
        camera.data.angle = toRad(fov)
    
        scene.objects.link(camera)
        
        
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
    
    for model_path in siloamSeeMaps:
        scene.camera = camera
        path = os.path.join(modelPath, model_path)
        # make a new scene with cam and lights linked
        context.screen.scene = scene
        bpy.ops.scene.new(type='LINK_OBJECTS')
        context.scene.name = model_path
        cams = [c for c in context.scene.objects if c.type == 'CAMERA']
        #import model
        bpy.ops.import_scene.obj(filepath=path, axis_forward='-Z', axis_up='Y', filter_glob="*.obj;*.mtl")
        
        bpy.data.scenes[model_path].render.resolution_x = 400
        bpy.data.scenes[model_path].render.resolution_y = 400
        bpy.data.scenes[model_path].render.resolution_percentage = 100
        
        for c in cams:
            context.scene.camera = c                                    
            print("Render ", model_path, context.scene.name, c.name)
            bpy.data.scenes[model_path].render.filepath = os.path.join(*["/Users/LordNelson/Documents/Work/LiverpoolUni/DissertationStore/2-learn/renders", folder, c.name])
            bpy.ops.render.render(write_still=True)