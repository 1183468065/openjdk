/*
 * Copyright (c) 2018, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

#import <Cocoa/Cocoa.h>
#import <JavaNativeFoundation/JavaNativeFoundation.h>

static NSWindow *testWindow;
static NSColorPanel *colorPanel;

/*
 * Class:     TestMainKeyWindow
 * Method:    setup
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_TestMainKeyWindow_setup(JNIEnv *env, jclass cl)
{
    JNF_COCOA_ENTER(env);

    void (^block)() = ^(){
        NSScreen *mainScreen = [NSScreen mainScreen];
        NSRect screenFrame = [mainScreen frame];
        NSRect frame = NSMakeRect(130, screenFrame.size.height - 280, 200, 100);

#if __MAC_OS_X_VERSION_MAX_ALLOWED >= 101200
        NSWindowStyleMask style = NSWindowStyleMaskTitled;
#else
        NSInteger style = NSTitledWindowMask;
#endif

        NSRect rect = [NSWindow contentRectForFrameRect:frame styleMask:style];
        NSBackingStoreType bt = NSBackingStoreBuffered;
        testWindow = [[[NSWindow alloc] initWithContentRect:rect styleMask:style backing:bt defer:NO] retain];
        testWindow.title = @"NSWindow";
        [testWindow setBackgroundColor:[NSColor blueColor]];
        [testWindow makeKeyAndOrderFront:nil];
        // Java coordinates are 100, 200

        colorPanel = [[NSColorPanel sharedColorPanel] retain];
        [colorPanel setReleasedWhenClosed: YES];
        colorPanel.restorable = NO;
        [colorPanel setFrameTopLeftPoint: NSMakePoint(130, screenFrame.size.height - 300)];
        // Java coordinates are 100, 400
        [colorPanel makeKeyAndOrderFront: nil];
    };

    if ([NSThread isMainThread]) {
        block();
    } else {
        [JNFRunLoop performOnMainThreadWaiting:YES withBlock:block];
    }

    JNF_COCOA_EXIT(env);
}

/*
 * Class:     TestMainKeyWindow
 * Method:    takedown
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_TestMainKeyWindow_takedown(JNIEnv *env, jclass cl)
{
    JNF_COCOA_ENTER(env);

    void (^block)() = ^(){
        if (testWindow != nil) {
            [testWindow close];
            testWindow = nil;
        }
        if (colorPanel != nil) {
            [colorPanel orderOut:nil];
            colorPanel = nil;
        }
    };

    if ([NSThread isMainThread]) {
        block();
    } else {
        [JNFRunLoop performOnMainThreadWaiting:YES withBlock:block];
    }

    JNF_COCOA_EXIT(env);
}

/*
 * Class:     TestMainKeyWindow
 * Method:    activateApplication
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_TestMainKeyWindow_activateApplication
  (JNIEnv *env, jclass cl)
{
    JNF_COCOA_ENTER(env);

    void (^block)() = ^(){
        [NSApp activateIgnoringOtherApps:YES];
    };

    [JNFRunLoop performOnMainThreadWaiting:YES withBlock:block];

  JNF_COCOA_EXIT(env);
}
