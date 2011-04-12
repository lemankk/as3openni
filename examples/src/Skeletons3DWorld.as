package
{
	import flash.events.Event;
	
	import mx.collections.ArrayCollection;
	
	import org.as3openni.objects.NiPoint3D;
	import org.as3openni.objects.NiSkeleton;
	import org.papervision3d.core.geom.Lines3D;
	import org.papervision3d.core.geom.renderables.Line3D;
	import org.papervision3d.core.geom.renderables.Vertex3D;
	import org.papervision3d.core.render.sort.NullSorter;
	import org.papervision3d.materials.special.LineMaterial;
	import org.papervision3d.view.BasicView;
	
	public class Skeletons3DWorld extends BasicView
	{
		private var _skeletonsCollection:ArrayCollection;
		private var _currentSkelBody:Lines3D;
		
		public function Skeletons3DWorld(viewportWidth:Number=640, viewportHeight:Number=480, scaleToStage:Boolean=true, interactive:Boolean=false, cameraType:String="Target")
		{
			super(viewportWidth, viewportHeight, scaleToStage, interactive, cameraType);
			this.renderer.renderScene(scene, camera, viewport);
			this._skeletonsCollection = new ArrayCollection();
		}
		
		public function addUser(user:Number):void
		{
			if(!this.checkIfUserExist(user))
			{
				var skelBody:Lines3D = new Lines3D(new LineMaterial());
				this.scene.addChild(skelBody);
				this._skeletonsCollection.addItem({user:user, skelBody:skelBody, skel:new NiSkeleton()});
			}
		}
		
		public function removeUser(user:Number):void
		{
			for(var i:Number = 0; i < this._skeletonsCollection.length; i++)
			{
				var eachUser:Object = this._skeletonsCollection.getItemAt(i);
				if(eachUser.user == user) 
				{
					var skelBody:Lines3D = eachUser.skelBody as Lines3D;
					this.scene.removeChild(skelBody);
					this._skeletonsCollection.removeItemAt(i);
				}
			}
		}
		
		public function updateUserSkeleton(skeleton:NiSkeleton):void
		{
			for(var i:Number = 0; i < this._skeletonsCollection.length; i++)
			{
				var eachUser:Object = this._skeletonsCollection.getItemAt(i);
				var skelBody:Lines3D = eachUser.skelBody as Lines3D;
				var skel:NiSkeleton = eachUser.skel as NiSkeleton;
				
				if(eachUser.user == skeleton.user) 
				{
					skel = skeleton;
					this.renderSkeleton(skelBody, skel);
				}
			}
		}
		
		/*override protected function onRenderTick(event:Event=null):void
		{
			for(var i:Number = 0; i < this._skeletonsCollection.length; i++)
			{
				var eachUser:Object = this._skeletonsCollection.getItemAt(i);
				var skelBody:Lines3D = eachUser.skelBody as Lines3D;
				var skel:NiSkeleton = eachUser.skel as NiSkeleton;
				this.renderSkeleton(skelBody, skel);
				this.renderer.renderScene(scene, camera, viewport);
			}
			
			super.onRenderTick(event);
		}*/
		
		private function renderSkeleton(skelBody:Lines3D, skeleton:NiSkeleton):void
		{
			if(skelBody && skeleton)
			{
				// Clear the lines.
				skelBody.removeAllLines();
				
				// Draw each body part starting with the Head and Neck.
				this.drawLine(skelBody, skeleton.head, skeleton.neck);
				
				// Left Arm.
				this.drawLine(skelBody, skeleton.neck, skeleton.leftShoulder);
				this.drawLine(skelBody, skeleton.leftShoulder, skeleton.leftElbow);
				this.drawLine(skelBody, skeleton.leftElbow, skeleton.leftHand);
				
				// Right Arm.
				this.drawLine(skelBody, skeleton.neck, skeleton.rightShoulder);
				this.drawLine(skelBody, skeleton.rightShoulder, skeleton.rightElbow);
				this.drawLine(skelBody, skeleton.rightElbow, skeleton.rightHand);
				
				// Torso.
				this.drawLine(skelBody, skeleton.leftShoulder, skeleton.torso);
				this.drawLine(skelBody, skeleton.rightShoulder, skeleton.torso);
				
				// Left Leg.
				this.drawLine(skelBody, skeleton.torso, skeleton.leftHip);
				this.drawLine(skelBody, skeleton.leftHip, skeleton.leftKnee);
				this.drawLine(skelBody, skeleton.leftKnee, skeleton.leftFoot);
				
				// Right Leg.
				this.drawLine(skelBody, skeleton.torso, skeleton.rightHip);
				this.drawLine(skelBody, skeleton.rightHip, skeleton.rightKnee);
				this.drawLine(skelBody, skeleton.rightKnee, skeleton.rightFoot);
				
				// And Hip.
				this.drawLine(skelBody, skeleton.leftHip, skeleton.rightHip);
			}
		}
		
		private function checkIfUserExist(user:Number):Boolean
		{
			var bool:Boolean = false;
			for(var i:Number = 0; i < this._skeletonsCollection.length; i++)
			{
				var eachUser:Object = this._skeletonsCollection.getItemAt(i);
				if(eachUser.user == user) bool = true;
			}
			return bool;
		}
		
		private function drawLine(skelBody:Lines3D, from:NiPoint3D, to:NiPoint3D):void
		{
			var line:Line3D;
			var v1:Vertex3D;
			var v2:Vertex3D;
			
			v1 = new Vertex3D(from.pointX, from.pointY+300, from.pointZ/5);
			v2 = new Vertex3D(to.pointX, to.pointY+300, to.pointZ/5);
			line = new Line3D(skelBody, new LineMaterial(0xFF0000) , 2, v1, v2);
			skelBody.addLine(line);
		}
	}
}