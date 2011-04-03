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
		public var skeleton:NiSkeleton;
		
		public function Skeletons3DWorld(viewportWidth:Number=640, viewportHeight:Number=480, scaleToStage:Boolean=true, interactive:Boolean=false, cameraType:String="Target")
		{
			super(viewportWidth, viewportHeight, scaleToStage, interactive, cameraType);
			this.renderer.renderScene(scene, camera, viewport);
			this._skeletonsCollection = new ArrayCollection();
		}
		
		public function checkIfUserExist(user:Number):Boolean
		{
			var bool:Boolean = false;
			for(var i:Number = 0; i < this._skeletonsCollection.length; i++)
			{
				var eachUser:Object = this._skeletonsCollection.getItemAt(i);
				if(eachUser.user == user) bool = true;
			}
			return bool;
		}
		
		public function addUser(user:Number):void
		{
			var skelBody:Lines3D = new Lines3D(new LineMaterial());
			this.scene.addChild(skelBody);
			this._skeletonsCollection.addItem({user:user, skelBody:skelBody});
		}
		
		public function removeUser(user:Number):void
		{
			for(var i:Number = 0; i < this._skeletonsCollection.length; i++)
			{
				var eachUser:Object = this._skeletonsCollection.getItemAt(i);
				if(eachUser.user == user) this._skeletonsCollection.removeItemAt(i);
			}
		}
		
		override protected function onRenderTick(event:Event=null):void
		{
			// Grab the current user's skeleton body.
			if(!this._currentSkelBody)
			{
				if(this.skeleton)
				{
					this._currentSkelBody = this.lookupSkeletonBody(this.skeleton.user);
				}
			}
			else
			{
				if(this._currentSkelBody)
				{
					this._currentSkelBody.removeAllLines();
					this.renderSkeleton();
					this.renderer.renderScene(scene, camera, viewport);
				}
			}
			
			super.onRenderTick(event);
		}
		
		private function renderSkeleton():void
		{
			if(this._currentSkelBody && skeleton)
			{
				// Draw each body part starting with the Head and Neck.
				this.drawLine(this._currentSkelBody, skeleton.head, skeleton.neck);
				
				// Left Arm.
				this.drawLine(this._currentSkelBody, skeleton.neck, skeleton.leftShoulder);
				this.drawLine(this._currentSkelBody, skeleton.leftShoulder, skeleton.leftElbow);
				this.drawLine(this._currentSkelBody, skeleton.leftElbow, skeleton.leftHand);
				
				// Right Arm.
				this.drawLine(this._currentSkelBody, skeleton.neck, skeleton.rightShoulder);
				this.drawLine(this._currentSkelBody, skeleton.rightShoulder, skeleton.rightElbow);
				this.drawLine(this._currentSkelBody, skeleton.rightElbow, skeleton.rightHand);
				
				// Torso.
				this.drawLine(this._currentSkelBody, skeleton.leftShoulder, skeleton.torso);
				this.drawLine(this._currentSkelBody, skeleton.rightShoulder, skeleton.torso);
				
				// Left Leg.
				this.drawLine(this._currentSkelBody, skeleton.torso, skeleton.leftHip);
				this.drawLine(this._currentSkelBody, skeleton.leftHip, skeleton.leftKnee);
				this.drawLine(this._currentSkelBody, skeleton.leftKnee, skeleton.leftFoot);
				
				// Right Leg.
				this.drawLine(this._currentSkelBody, skeleton.torso, skeleton.rightHip);
				this.drawLine(this._currentSkelBody, skeleton.rightHip, skeleton.rightKnee);
				this.drawLine(this._currentSkelBody, skeleton.rightKnee, skeleton.rightFoot);
				
				// And Hip.
				this.drawLine(this._currentSkelBody, skeleton.leftHip, skeleton.rightHip);
			}
		}
		
		private function lookupSkeletonBody(user:Number):Lines3D
		{
			var skelBody:Lines3D = null;
			for(var i:Number = 0; i < this._skeletonsCollection.length; i++)
			{
				var eachUser:Object = this._skeletonsCollection.getItemAt(i);
				if(eachUser.user == user) skelBody = eachUser.skelBody;
			}
			return skelBody;
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