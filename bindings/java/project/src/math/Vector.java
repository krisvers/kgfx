package math;

import java.util.Arrays;

public class Vector {
	public float[] values;

	public Vector(int len) {
		this.values = new float[len];
		Arrays.fill(this.values, 0);
	}

	public Vector(float[] vector) {
		this.values = new float[vector.length];
		System.arraycopy(vector, 0, this.values, 0, vector.length);
	}

	public Vector(float x, float y) {
		this.values = new float[2];
		this.values[0] = x;
		this.values[1] = y;
	}

	public Vector(float x, float y, float z) {
		this.values = new float[3];
		this.values[0] = x;
		this.values[1] = y;
		this.values[2] = z;
	}

	public Vector(float x, float y, float z, float w) {
		this.values = new float[4];
		this.values[0] = x;
		this.values[1] = y;
		this.values[2] = z;
		this.values[3] = w;
	}

	private void check() {
		if (this.values == null) {
			throw new RuntimeException("Vector not initialized");
		}
	}

	private static void check(Vector vector) {
		if (vector == null) {
			throw new RuntimeException("Vector not initialized");
		}
	}

	private static void check(float[] vector) {
		if (vector == null) {
			throw new RuntimeException("Vector not initialized");
		}
	}

	public void zero() {
		check();
		Arrays.fill(this.values, 0);
	}

	public Vector add(Vector vector) {
		check();
		check(vector);

		if (vector.values.length != this.values.length) {
			throw new RuntimeException("Vector lengths do not match");
		}

		Vector n = new Vector(this.values.length);

		for (int i = 0; i < this.values.length; ++i) {
			n.values[i] = this.values[i] + vector.values[i];
		}

		return n;
	}

	public Vector sub(Vector vector) {
		check();
		check(vector);

		if (vector.values.length != this.values.length) {
			throw new RuntimeException("Vector lengths do not match");
		}

		Vector n = new Vector(this.values.length);

		for (int i = 0; i < this.values.length; ++i) {
			n.values[i] = this.values[i] - vector.values[i];
		}

		return n;
	}

	public Vector mul(Vector vector) {
		check();
		check(vector);

		if (vector.values.length != this.values.length) {
			throw new RuntimeException("Vector lengths do not match");
		}

		Vector n = new Vector(this.values.length);

		for (int i = 0; i < this.values.length; ++i) {
			n.values[i] = this.values[i] * vector.values[i];
		}

		return n;
	}

	public float mulInner(Vector vector) {
		check();
		check(vector);

		if (vector.values.length != this.values.length) {
			throw new RuntimeException("Vector lengths do not match");
		}

		float p = 0;
		for (int i = 0; i < this.values.length; ++i) {
			p += this.values[i] * vector.values[i];
		}

		return p;
	}

	public Vector mulCross(Vector vector) {
		check();
		check(vector);

		if (vector.values.length != this.values.length || this.values.length != 3) {
			throw new RuntimeException("Vector lengths are not 3");
		}

		Vector n = new Vector(this.values.length);
		n.values[0] = this.values[1] * vector.values[2] - this.values[2] * vector.values[1];
		n.values[1] = this.values[2] * vector.values[0] - this.values[0] * vector.values[2];
		n.values[2] = this.values[0] * vector.values[1] - this.values[1] * vector.values[0];

		return n;
	}

	public Vector div(Vector vector) {
		check();
		check(vector);

		if (vector.values.length != this.values.length) {
			throw new RuntimeException("Vector lengths do not match");
		}

		Vector n = new Vector(this.values.length);

		for (int i = 0; i < this.values.length; ++i) {
			n.values[i] = this.values[i] / vector.values[i];
		}

		return n;
	}

	public Vector scale(float scalar) {
		check();

		Vector n = new Vector(this.values.length);

		for (int i = 0; i < this.values.length; ++i) {
			n.values[i] = this.values[i] * scalar;
		}

		return n;
	}

	public float magnitude() {
		check();

		float sum = 0;
		for (float value : this.values) {
			sum += value * value;
		}

		return (float) Math.sqrt(sum);
	}

	public Vector normalized() {
		float mag = magnitude();
		if (mag == 0) {
			Vector vector = new Vector(this.values.length);
			vector.set(this);
			return vector;
		}

		Vector vector = new Vector(this.values.length);
		for (int i = 0; i < this.values.length; ++i) {
			vector.values[i] = this.values[i] / mag;
		}

		return vector;
	}

	public void normalize() {
		float mag = magnitude();
		if (mag == 0) {
			return;
		}

		for (int i = 0; i < this.values.length; ++i) {
			this.values[i] /= mag;
		}
	}

	public void set(float[] vector) {
		check();
		check(vector);

		if (vector.length != this.values.length) {
			throw new RuntimeException("Vector lengths do not match");
		}

		System.arraycopy(vector, 0, this.values, 0, this.values.length);
	}

	public void set(Vector vector) {
		check(vector);
		set(vector.values);
	}

	public Vector copy() {
		check();
		Vector vector = new Vector(this.values.length);
		vector.set(this);
		return vector;
	}

	@Override
	public boolean equals(Object obj) {
		if (!(obj instanceof Vector vector)) {
			return false;
		}
		if (this == obj) {
			return true;
		}

		if (vector.values.length != this.values.length) {
			return false;
		}

		for (int i = 0; i < this.values.length; ++i) {
			if (this.values[i] != vector.values[i]) {
				return false;
			}
		}

		return true;
	}

	@Override
	public String toString() {
		check();
		StringBuilder str = new StringBuilder();
		for (int i = 0; i < this.values.length; ++i) {
			str.append(this.values[i]);
			if (i < this.values.length - 1) {
				str.append(", ");
			}
		}
		return str.toString();
	}
}
