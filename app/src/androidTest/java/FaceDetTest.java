import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Point;
import android.support.test.runner.AndroidJUnit4;
import android.test.suitebuilder.annotation.SmallTest;

import com.tzutalin.dlib.Constants;
import com.tzutalin.dlib.FaceDet;
import com.tzutalin.dlib.VisionDetRet;

import junit.framework.Assert;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;

import java.util.ArrayList;
import java.util.List;

import static org.hamcrest.Matchers.notNullValue;
import static org.hamcrest.core.Is.is;
import static org.junit.Assert.assertThat;

/**
 * Created by houzhi on 16-10-20.
 */
@RunWith(AndroidJUnit4.class)
@SmallTest
public class FaceDetTest {

    @Before
    public void setup() {
    }

    @After
    public void tearDown() {
    }

    @Test
    public void testDetBitmapFace() {
        FaceDet faceDet = new FaceDet(Constants.getFaceShapeModelPath());
        Bitmap bitmap = BitmapFactory.decodeFile("/sdcard/test.jpg");
        assertThat(bitmap, notNullValue());
        List<VisionDetRet> results = faceDet.detect(bitmap);
        for (final VisionDetRet ret : results) {
            String label = ret.getLabel();
            int rectLeft = ret.getLeft();
            int rectTop = ret.getTop();
            int rectRight = ret.getRight();
            int rectBottom = ret.getBottom();
            assertThat(label, is("face"));
            Assert.assertTrue(rectLeft > 0);
        }
        faceDet.release();
    }

    @Test
    public void testDetFace() {
        Bitmap bitmap = BitmapFactory.decodeFile("/sdcard/test.jpg");
        assertThat(bitmap, notNullValue());
        FaceDet faceDet = new FaceDet(Constants.getFaceShapeModelPath());
        List<VisionDetRet> results = faceDet.detect("/sdcard/test.jpg");
        for (final VisionDetRet ret : results) {
            String label = ret.getLabel();
            int rectLeft = ret.getLeft();
            int rectTop = ret.getTop();
            int rectRight = ret.getRight();
            int rectBottom = ret.getBottom();
            assertThat(label, is("face"));
            Assert.assertTrue(rectLeft > 0);
        }
        faceDet.release();
    }

    @Test
    public void testDetBitmapFaceLandmarkDect() {
        Bitmap bitmap = BitmapFactory.decodeFile("/sdcard/test.jpg");
        assertThat(bitmap, notNullValue());
        FaceDet faceDet = new FaceDet(Constants.getFaceShapeModelPath());
        List<VisionDetRet> results = faceDet.detect(bitmap);
        for (final VisionDetRet ret : results) {
            String label = ret.getLabel();
            int rectLeft = ret.getLeft();
            int rectTop = ret.getTop();
            int rectRight = ret.getRight();
            int rectBottom = ret.getBottom();
            ArrayList<Point> landmarks = ret.getFaceLandmarks();
            assertThat(label, is("face"));
            Assert.assertTrue(landmarks.size() > 0);
            Assert.assertTrue(rectLeft > 0);
        }
        faceDet.release();
    }

    @Test
    public void testDetFaceLandmark() {
        FaceDet faceDet = new FaceDet(Constants.getFaceShapeModelPath());
        List<VisionDetRet> results = faceDet.detect("/sdcard/test.jpg");
        for (final VisionDetRet ret : results) {
            String label = ret.getLabel();
            int rectLeft = ret.getLeft();
            int rectTop = ret.getTop();
            int rectRight = ret.getRight();
            int rectBottom = ret.getBottom();
            ArrayList<Point> landmarks = ret.getFaceLandmarks();
            assertThat(label, is("face"));
            Assert.assertTrue(landmarks.size() > 0);
            Assert.assertTrue(rectLeft > 0);
        }
        faceDet.release();
    }
}
